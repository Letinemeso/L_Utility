#pragma once

#include <ostream>

#include <vec2.hpp>
#include <vec3.hpp>


std::ostream& operator<<(std::ostream& _stream, const glm::vec2& _vec);

std::ostream& operator<<(std::ostream& _stream, const glm::vec3& _vec);
