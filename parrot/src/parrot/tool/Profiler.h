#pragma once

#include <vector>

namespace parrot {

    struct ProfileResult {
        const char* name;
        float       time;
    };

    class Profiler {
    public:
        static const std::vector<ProfileResult>& getResult() { return m_profile_result_list; }
        static void addResult(ProfileResult& profile_result) { m_profile_result_list.push_back(profile_result); }
        static void clear() { m_profile_result_list.clear();  }
    private:
        static std::vector<ProfileResult> m_profile_result_list;
    };
}

#define PROFILE_SCOPE(name) parrot::ScopeTimer timer##__LINE__(name, [](parrot::ProfileResult profile_result) { parrot::Profiler::addResult(profile_result); });