#pragma once

#include "Engine/Core/Base.h"
#include "Engine/Core/Timestep.h"
#include "Engine/Events/Event.h"

#include <string>

namespace Engine
{
    class Layer
    {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(Timestep timestep) {}
        virtual void OnImGuiRender() {}
        virtual void OnEvent(Event& event) {}

        inline const std::string& GetName() const { return m_DebugName; }

    protected:
        std::string m_DebugName;
    };
}