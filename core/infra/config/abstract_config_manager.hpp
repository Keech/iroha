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

#ifndef IROHA_CONFIG_H
#define IROHA_CONFIG_H

#include <fstream>  // ifstream, ofstream
#include <util/logger.hpp>
#include <util/use_optional.hpp>
#include <vendor/json/src/json.hpp>

using json = nlohmann::json;

namespace config {

class AbstractConfigManager {
 protected:

  optional<json> openConfig(const std::string& configName) {
    if (_configData) {  // content is already loaded
      return _configData;
    }


    auto iroha_home = getenv("IROHA_HOME");
    if (iroha_home == nullptr) {
      logger::error("config") << "Set environment variable IROHA_HOME";
      exit(EXIT_FAILURE);
    }

    auto configFolderPath = std::string(iroha_home) + "/";
    auto jsonStr = openJSONText(configFolderPath + configName);

    logger::debug("config") << "load json is " << jsonStr;

    setConfigData(std::move(jsonStr));

    return _configData;
  }


  std::string openJSONText(const std::string& PathToJSONFile) {
    std::ifstream ifs(PathToJSONFile);
    if (ifs.fail()) {
      logger::error("config") << "Not found: " << PathToJSONFile;
      return nullptr;
    }

    std::istreambuf_iterator<char> it(ifs);
    return std::string(it, std::istreambuf_iterator<char>());
  }


  void setConfigData(std::string&& jsonStr) {
    try {
      _configData = json::parse(std::move(jsonStr));
    } catch (...) {
      logger::error("config") << "Can't parse json: " << getConfigName();
    }
  }


 public:
  virtual std::string getConfigName() = 0;
  optional<json> getConfigData() { return this->_configData; }

 protected:
  optional<json> _configData;
};
}

#endif  // IROHA_CONFIG_H
