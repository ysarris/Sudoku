#pragma once
#ifndef SIZE_STATUS
#define SIZE_STATUS

enum class SizeStatus { GROWING, SHRINKING };
static constexpr auto DEFAULT_SIZE_STATUS = SizeStatus::GROWING;

#endif