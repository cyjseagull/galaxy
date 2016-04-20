// Copyright (c) 2016, Baidu.com, Inc. All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#pragma once

#include <map>
#include <set>
#include <list>
#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>
#include "src/protocol/galaxy.pb.h"

namespace baidu {
namespace galaxy {
namespace sched {

typedef std::string AgentEndpoint;
typedef std::string ContainerGroupId;
typedef std::string ContainerId;

enum ContainerStatus {
    kPending = 1,
    kAllocating = 2,
    kRunning = 3,
    kError = 4,
    kDestroying = 5,
    kTerminated = 6
};

enum ResourceError {
    kNoCpu = 1,
    kNoMemory = 2,
    kNoMedium = 3,
    kNoDevice = 4,
    kNoPort = 5,
    kPortConflict = 6,
    kLabelMismatch = 7,
    kNoMemoryForTmpfs = 8
};

struct Resource {
    proto::CpuRequired cpu;
    proto::MemoryRequired memory;
    std::vector<proto::VolumRequired> volums;
    std::map<std::string, proto::PortRequired> ports;
};

struct Requirement {
    std::string label;
    Resource res;
};

struct Container {
    ContainerStatus status;
    ContainerId id;
    ContainerGroupId group_id;
    Requirement require;
    Resource allocated;
    typedef boost::shared_ptr<Container> Ptr;
};

struct ContainerGroup {
    ContainerGroupId id;
    std::vector<Container::Ptr> containers;
    std::list<Container::Ptr> pending_queue;
    typedef boost::shared_ptr<ContainerGroup> Ptr;
};

class Agent {
public:
    explicit Agent(const AgentEndpoint endpoint,
                   const Resource& res_total);
    bool CanPut(const Container* container, ResourceError& err);
    void Put(Container::Ptr container);
    void Evict(Container::Ptr container);
    typedef boost::shared_ptr<Agent> Ptr;
private:
    AgentEndpoint endpoint_;
    std::set<std::string> labels_;
    Resource res_total_;
    Resource res_assigned_;
    std::vector<Container::Ptr> containers_;
};

class Scheduler {
public:
    explicit Scheduler();
    void AddAgent(Agent::Ptr agent);
    void RemoveAgent(const AgentEndpoint& endpoint);
    ContainerGroupId Submit(const Requirement& require, int replica);
    void Kill(const ContainerGroupId& group_id);
    void ScaleUp(const ContainerGroupId& group_id, int replica);
    void ScaleDown(const ContainerGroupId& group_id, int replica);
    //start the main schueduling loop
    void Start();
    //
    void ShowAssignment(const AgentEndpoint& endpoint,
                        std::vector<Container::Ptr>& containers);
    void ShowContainerGroup(const ContainerGroupId group_id,
                            std::vector<Container::Ptr>& containers);
    void ChangeStatus(const ContainerId& container_id, ContainerStatus status);

private:
    std::map<ContainerId, Container::Ptr> containers_;
    std::map<AgentEndpoint, Agent::Ptr> agent_assign_;
    std::map<ContainerGroupId, ContainerGroup::Ptr> container_groups_;
};

} //namespace sched
} //namespace galaxy
} //namespace baidu
