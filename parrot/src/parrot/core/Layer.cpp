#include "prpch.h"
#include "parrot/core/Layer.h"

namespace parrot {

    Layer::Layer(const std::string& debug_name) : m_debug_name(debug_name) {}

    Layer::~Layer() {}

}