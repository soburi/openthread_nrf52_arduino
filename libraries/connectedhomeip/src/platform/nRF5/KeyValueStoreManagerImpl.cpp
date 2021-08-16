/*
 *
 *    Copyright (c) 2021 Project CHIP Authors
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

/**
 *    @file
 *          Platform-specific key value storage implementation for K32W
 */
/* this file behaves like a config.h, comes first */
#include <platform/internal/CHIPDeviceLayerInternal.h>

#include "support/CHIPMem.h"
#include <platform/nRF5/nRF5Config.h>
#include <platform/KeyValueStoreManager.h>
#include <string>

namespace chip {
namespace DeviceLayer {
namespace PersistedStorage {

/* TODO: adjust this value */
#define MAX_NO_OF_KEYS 255

KeyValueStoreManagerImpl KeyValueStoreManagerImpl::sInstance;

CHIP_ERROR KeyValueStoreManagerImpl::_Get(const char * key, void * value, size_t value_size, size_t * read_bytes_size,
                                          size_t offset_bytes)
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    //std::hash<std::string> hash_fn;
    uint16_t key_id = chip::DeviceLayer::Internal::NRF5Config::kFileId_KVS;
    size_t read_bytes;

    VerifyOrExit((key != NULL) && (value != NULL), err = CHIP_ERROR_INVALID_ARGUMENT);

    ChipLogProgress(DeviceLayer, "KVS, get key id:: %i", key_id);

    err = chip::DeviceLayer::Internal::NRF5Config::ReadConfigValueBin(
        chip::DeviceLayer::Internal::NRF5ConfigKey(0, 0), (uint8_t *) value, value_size, read_bytes);

    *read_bytes_size = read_bytes;

exit:
    return err;
}

CHIP_ERROR KeyValueStoreManagerImpl::_Put(const char * key, const void * value, size_t value_size)
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    uint16_t key_id = chip::DeviceLayer::Internal::NRF5Config::kFileId_KVS;

    VerifyOrExit((key != NULL) && (value != NULL), err = CHIP_ERROR_INVALID_ARGUMENT);
    //key_id = hash_fn(key) % MAX_NO_OF_KEYS;

    ChipLogProgress(DeviceLayer, "KVS, put key id:: %i", key_id);

    err = chip::DeviceLayer::Internal::NRF5Config::WriteConfigValueBin(
        chip::DeviceLayer::Internal::NRF5ConfigKey(0, 0), (uint8_t *) value, value_size);

exit:
    return err;
}

CHIP_ERROR KeyValueStoreManagerImpl::_Delete(const char * key)
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    uint16_t key_id = chip::DeviceLayer::Internal::NRF5Config::kFileId_KVS;

    VerifyOrExit(key != NULL, err = CHIP_ERROR_INVALID_ARGUMENT);
//    key_id = hash_fn(key) % MAX_NO_OF_KEYS;

    ChipLogProgress(DeviceLayer, "KVS, deleting key id:: %i", key_id);

    err = chip::DeviceLayer::Internal::NRF5Config::ClearConfigValue(chip::DeviceLayer::Internal::NRF5ConfigKey(0, 0));

exit:
    return err;
}

} // namespace PersistedStorage
} // namespace DeviceLayer
} // namespace chip
