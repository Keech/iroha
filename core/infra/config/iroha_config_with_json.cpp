/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "iroha_config_with_json.hpp"

namespace config {
    IrohaConfigManager::IrohaConfigManager() { }

    IrohaConfigManager& IrohaConfigManager::getInstance() {
        static IrohaConfigManager manager;
        return manager;
    }

    std::string IrohaConfigManager::getDBPath() {
        if (auto config = openConfig("config.json")) {
            return (*config).value("database_path", "/tmp");
        }
        return "/tmp";
    }
}
