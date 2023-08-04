#pragma once

#include <string>


namespace Maconomy {

    // ----- Toggl Json tags ----------------------------------

    // Time entries array key.
    const std::string TOGGL_JSON_ARRAY = "TimeEntries";

    // Start date (used for determining weekday).
    const std::string TOGGL_JSON_DATE = "StartDate";

    // Daily description.
    const std::string TOGGL_JSON_DESCRIPTION = "Description";

    // Task duration.
    const std::string TOGGL_JSON_DURATION = "Duration";

    // Name of the project.
    const std::string TOGGL_JSON_PROJECT = "ProjectName";

    // Name of the client.
    const std::string TOGGL_JSON_CLIENT = "ClientName";

    // Tag key.
    const std::string TOGGL_JSON_TAG = "TagNames";

    // Job delimiter.
    const char TOGGL_JOB_DELIMITER = ';';
    
    // ----- Optimize html constants ---------------------------

    // Start row for time log entries.
    const int OPTIMIZE_START = 35;

    // Task start and end tags.
    const std::string OPTIMIZE_TASK_START = "<B>";
    const std::string OPTIMIZE_TASK_END = "</B>";

    // Job delimiter (used for splitting).
    const char OPTIMIZE_JOB_DELIMITER = ',';

    // Task desctiption delimiter.
    const char OPTIMIZE_TASK_DESC_DELIMITER = ';';

    // Time entry start and end tags.
    const std::string OPTIMIZE_TIME_START = "<CENTER>";
    const std::string OPTIMIZE_TIME_END = "</CENTER>";

    // Empty time log entry value.
    const std::string OPTIMIZE_TIME_EMPTY = "-";

    // End row tag.
    const std::string OPTIMIZE_TOTAL = "Total:";

}