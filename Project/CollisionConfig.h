#pragma once

#include <cstdint>

//プレイヤー陣営の属性:0x00000001
const uint32_t collisionAttributePlayer = 0b1;
//敵陣営の属性:0x00000010
const uint32_t collisionAttributeEnemy = 0b1 << 1;

