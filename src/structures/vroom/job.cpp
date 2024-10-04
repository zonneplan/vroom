/*

This file is part of VROOM.

Copyright (c) 2015-2024, Julien Coupey.
All rights reserved (see LICENSE).

*/

#include "structures/vroom/job.h"
#include "utils/helpers.h"

namespace vroom {

Job::Job(Id id,
         const Location& location,
         UserDuration setup,
         UserDuration service,
         UserDurationMap service_per_vehicle_type,
         Amount delivery,
         Amount pickup,
         Skills skills,
         Priority priority,
         const std::vector<TimeWindow>& tws,
         std::string description,
         const std::optional<std::string>& task_type)
  : location(location),
    id(id),
    type(JOB_TYPE::SINGLE),
    setup(utils::scale_from_user_duration(setup)),
    service(utils::scale_from_user_duration(service)),
    service_per_vehicle_type(
      utils::scale_from_user_duration_map(service_per_vehicle_type)),
    delivery(std::move(delivery)),
    pickup(std::move(pickup)),
    skills(std::move(skills)),
    priority(priority),
    tws(tws),
    description(std::move(description)),
    task_type(task_type) {
  utils::check_tws(tws, id, "job");
  utils::check_priority(priority, id, "job");
}

Job::Job(Id id,
         JOB_TYPE type,
         const Location& location,
         UserDuration setup,
         UserDuration service,
         UserDurationMap service_per_vehicle_type,
         const Amount& amount,
         Skills skills,
         Priority priority,
         const std::vector<TimeWindow>& tws,
         std::string description,
         const std::optional<std::string>& task_type)
  : location(location),
    id(id),
    type(type),
    setup(utils::scale_from_user_duration(setup)),
    service(utils::scale_from_user_duration(service)),
    service_per_vehicle_type(
      utils::scale_from_user_duration_map(service_per_vehicle_type)),
    delivery((type == JOB_TYPE::DELIVERY) ? amount : Amount(amount.size())),
    pickup((type == JOB_TYPE::PICKUP) ? amount : Amount(amount.size())),
    skills(std::move(skills)),
    priority(priority),
    tws(tws),
    description(std::move(description)),
    task_type(task_type) {
  assert(type == JOB_TYPE::PICKUP || type == JOB_TYPE::DELIVERY);
  utils::check_tws(tws, id, "job");
  utils::check_priority(priority, id, "job");
}

bool Job::is_valid_start(Duration time) const {
  bool valid = false;

  for (const auto& tw : tws) {
    if (tw.contains(time)) { //
      valid = true;
      break;
    }
  }

  return valid;
}

} // namespace vroom
