// Copyright (c) 2016, Baidu.com, Inc. All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BAIDU_GALAXY_CLIENT_JOB_ACTION_H
#define BAIDU_GALAXY_CLIENT_JOB_ACTION_H

#include "galaxy_util.h"
#include <gflags/gflags.h>
#include "sdk/galaxy_sdk_appmaster.h"

namespace baidu {
namespace galaxy {
namespace client {

class JobAction {

public:
    explicit JobAction(const std::string& name, const std::string& token);
    ~JobAction();
    bool SubmitJob(const std::string& json_file);
    bool UpdateJob(const std::string& json_file, const std::string& jobid);
    bool StopJob(const std::string& jobid);
    bool RemoveJob(const std::string& jobid);
    bool ListJobs();
    bool ShowJob(const std::string& jobid);
    bool ExecuteCmd(const std::string& jobid, const std::string& cmd);

private:
    bool Init();
private:
    ::baidu::galaxy::sdk::AppMaster* app_master_;
    ::baidu::galaxy::sdk::User user_;

}; // end class JobAction

} // end namespace client
} // end namespace galaxy
} // end namespace baidu

#endif  // BAIDU_GALAXY_CLIENT_JOB_ACTION_H

/* vim: set ts=4 sw=4 sts=4 tw=100 */
