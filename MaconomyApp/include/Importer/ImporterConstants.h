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
    const std::string TOGGL_JSON_CLIENT = "Client";

    // Tag key.
    const std::string TOGGL_JSON_TAG = "TagNames";
}


/*
   "Description": "Sked",
      "StartDate": "2023-07-29",
      "StartTime": "17:16:26",
      "EndDate": "2023-07-29",
      "EndTime": "17:16:37",
      "Duration": "00:00:11",
      "IsBillable": true,
      "ProjectName": "Nobia Swe",
      "ClientName": "Client1",
      "TagNames": []
*/