#ifndef MAX_TYPE_TASKS_H
#define MAX_TYPE_TASKS_H

/*

This file is part of VROOM.

Copyright (c) 2015-2024, Julien Coupey.
All rights reserved (see LICENSE).

*/

#include <tuple>

#include "structures/typedefs.h"
#include "structures/vroom/job.h"
#include "structures/vroom/vehicle.h"

namespace vroom {

struct MaxTypeTasks {
  MaxTasksMap max_tasks_per_type;

  MaxTypeTasks() : max_tasks_per_type{} {};

  MaxTypeTasks(MaxTasksMap max_tasks_per_type)
    : max_tasks_per_type(max_tasks_per_type) {};

  MaxTypeTasks(std::vector<vroom::Job> jobs,
               const std::optional<Index> begin = std::optional<Index>(),
               const std::optional<Index> end = std::optional<Index>())
    : max_tasks_per_type{} {
    const Index begin_value = begin.value_or(0);
    const Index end_value = end.value_or(jobs.size());

    for (Index i = begin_value; i < end_value; ++i) {
      const Job& job = jobs[i];

      if (!job.task_type.has_value()) {
        continue;
      }

      const std::string task_type = job.task_type.value();

      if (max_tasks_per_type.find(task_type) == max_tasks_per_type.end()) {
        max_tasks_per_type[task_type] = 0;
      }

      max_tasks_per_type[task_type]++;
    }
  }

  MaxTypeTasks add(const Job& job, int64_t amount = 1) const {
    return add(job.task_type, amount);
  }

  MaxTypeTasks add(std::optional<std::string> task_type,
                   int64_t amount = 1) const {
    MaxTypeTasks new_max_tasks = *this;

    if (!task_type.has_value()) {
      return new_max_tasks;
    }

    const std::string type = task_type.value();

    if (new_max_tasks.max_tasks_per_type.find(type) ==
        new_max_tasks.max_tasks_per_type.end()) {
      new_max_tasks.max_tasks_per_type[type] = 0;
    }

    new_max_tasks.max_tasks_per_type[type] += amount;

    return new_max_tasks;
  }

  MaxTypeTasks sub(const Job& job, int64_t amount = 1) const {
    return sub(job.task_type, amount);
  }

  MaxTypeTasks sub(std::optional<std::string> task_type,
                   int64_t amount = 1) const {
    return add(task_type, -amount);
  }

  MaxTypeTasks reset_negatives() const {
    MaxTypeTasks new_max_tasks = *this;

    for (auto& type : new_max_tasks.max_tasks_per_type) {
      if (type.second < 0) {
        type.second = 0;
      }
    }

    return new_max_tasks;
  }

  bool exceeds_for_vehicle(const Vehicle& vehicle) const {
    for (const auto& type : vehicle.max_tasks_per_job_type) {
      if (max_tasks_per_type.at(type.first) > type.second) {
        return true;
      }
    }

    return false;
  }

  MaxTypeTasks& operator+=(const MaxTypeTasks& rhs) {
    for (const auto& type : rhs.max_tasks_per_type) {
      max_tasks_per_type[type.first] += type.second;
    }

    return *this;
  }

  MaxTypeTasks& operator-=(const MaxTypeTasks& rhs) {
    for (const auto& type : rhs.max_tasks_per_type) {
      max_tasks_per_type[type.first] -= type.second;
    }

    return *this;
  }

  MaxTypeTasks operator-() const {
    MaxTypeTasks opposite;

    for (const auto& type : max_tasks_per_type) {
      opposite.max_tasks_per_type[type.first] = -type.second;
    }

    return opposite;
  }

  friend MaxTypeTasks operator+(MaxTypeTasks lhs, const MaxTypeTasks& rhs) {
    lhs += rhs;
    return lhs;
  }

  friend MaxTypeTasks operator-(MaxTypeTasks lhs, const MaxTypeTasks& rhs) {
    lhs -= rhs;
    return lhs;
  }
};

} // namespace vroom

#endif
