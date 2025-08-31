#pragma once

#include <string>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <mutex>

namespace Engine {

	// Single trace event (Chrome Tracing format)
	struct ProfileResult {
		std::string Name;
		long long   Start;    // microseconds since epoch
		long long   End;      // microseconds since epoch
		uint32_t    ThreadID; // hashed thread id
	};

	// Session metadata
	struct InstrumentationSession {
		std::string Name;
	};

	// Minimal, file-based tracing sink producing Chrome Tracing JSON.
	// Thread-safe writes; only one active session at a time.
	class Instrumentor {
	public:
		Instrumentor() = default;
		~Instrumentor() { EndSession(); }

		// Begins a new profiling session. If another session is open, it is closed first.
		void BeginSession(const std::string& name, const std::string& filepath = "trace.json") {
			std::scoped_lock lock(m_Mutex);

			// Gracefully end a previous session if the user forgot to close it
			if (m_CurrentSession) {
				InternalEndSession();
			}

			m_Output.open(filepath, std::ios::out | std::ios::trunc);
			if (!m_Output.is_open()) {
				// If opening fails, leave no session active
				m_CurrentSession = nullptr;
				m_ProfileCount = 0;
				return;
			}

			m_CurrentSession = new InstrumentationSession{ name };
			m_ProfileCount = 0;
			WriteHeader();
		}

		// Ends the current session (if any).
		void EndSession() {
			std::scoped_lock lock(m_Mutex);
			InternalEndSession();
		}

		// Emits a single event entry.
		void WriteProfile(const ProfileResult& result) {
			std::scoped_lock lock(m_Mutex);
			if (!m_CurrentSession || !m_Output.is_open()) return;

			if (m_ProfileCount++ > 0)
				m_Output << ",";

			std::string safeName = result.Name;
			std::replace(safeName.begin(), safeName.end(), '"', '\'');

			m_Output << "{";
			m_Output << R"("cat":"function",)";
			m_Output << R"("dur":)" << (result.End - result.Start) << ",";
			m_Output << R"("name":")" << safeName << R"(",)";
			m_Output << R"("ph":"X",)";
			m_Output << R"("pid":0,)";
			m_Output << R"("tid":)" << result.ThreadID << ",";
			m_Output << R"("ts":)" << result.Start;
			m_Output << "}";

			m_Output.flush();
		}

		// Global singleton accessor
		static Instrumentor& Get() {
			static Instrumentor s_Instance;
			return s_Instance;
		}

	private:
		void InternalEndSession() {
			if (m_CurrentSession) {
				WriteFooter();
				m_Output.close();
				delete m_CurrentSession;
				m_CurrentSession = nullptr;
				m_ProfileCount = 0;
			}
		}

		void WriteHeader() {
			m_Output << R"({"otherData":{},"traceEvents":[)";
			m_Output.flush();
		}

		void WriteFooter() {
			m_Output << "]}";
			m_Output.flush();
		}

	private:
		std::mutex               m_Mutex;
		InstrumentationSession* m_CurrentSession = nullptr;
		std::ofstream            m_Output;
		int                      m_ProfileCount = 0;
	};

	// RAII timer that writes a trace event on destruction.
	class InstrumentationTimer {
	public:
		explicit InstrumentationTimer(const char* name)
			: m_Name(name) {
			m_Start = std::chrono::high_resolution_clock::now();
		}

		~InstrumentationTimer() { Stop(); }

		void Stop() {
			if (m_Stopped) return;
			const auto end = std::chrono::high_resolution_clock::now();

			const auto startUs = std::chrono::time_point_cast<std::chrono::microseconds>(m_Start).time_since_epoch().count();
			const auto endUs = std::chrono::time_point_cast<std::chrono::microseconds>(end).time_since_epoch().count();
			const uint32_t tid = static_cast<uint32_t>(std::hash<std::thread::id>{}(std::this_thread::get_id()));

			Instrumentor::Get().WriteProfile({ m_Name, startUs, endUs, tid });
			m_Stopped = true;
		}

	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start{};
		bool m_Stopped = false;
	};

} // namespace Engine

// -----------------------------------------------------------------------------
// Macros for convenient profiling usage
// -----------------------------------------------------------------------------
#define EG_PROFILE 1

#if EG_PROFILE
#define EG_PROFILE_BEGIN_SESSION(name, filepath) ::Engine::Instrumentor::Get().BeginSession(name, filepath)
#define EG_PROFILE_END_SESSION()                 ::Engine::Instrumentor::Get().EndSession()
#define EG_PROFILE_SCOPE(name)                   ::Engine::InstrumentationTimer EG_CONCAT(_egProfileTimer_, __LINE__){ name }
#ifdef _MSC_VER
#define EG_PROFILE_FUNCTION()                EG_PROFILE_SCOPE(__FUNCSIG__)
#else
#define EG_PROFILE_FUNCTION()                EG_PROFILE_SCOPE(__PRETTY_FUNCTION__)
#endif
// Helper for unique variable names per line
#define EG_CONCAT_INNER(a, b) a##b
#define EG_CONCAT(a, b)       EG_CONCAT_INNER(a, b)
#else
#define EG_PROFILE_BEGIN_SESSION(name, filepath)
#define EG_PROFILE_END_SESSION()
#define EG_PROFILE_SCOPE(name)
#define EG_PROFILE_FUNCTION()
#endif
