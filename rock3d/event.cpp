/*
 * rock3d.cpp: A 3D game engine for making retro FPS games
 * Copyright (C) 2018 Lexi Mayfield <alexmax2742@gmail.com>
 */

/**
 * @brief Event queue for inputs and such.
 */

#include "rock3d.h"

namespace rock3d
{

//******************************************************************************

class EventQueueImpl final : public EventQueue
{
    static constexpr size_t QUEUE_SIZE = 1024;
    static constexpr size_t QUEUE_MASK = QUEUE_SIZE - 1;

    std::array<rockEvent_t, QUEUE_SIZE> m_ncEvents;
    size_t m_qwHead = 0;
    size_t m_qwTail = 0;

    static auto Next(const size_t qwIndex) -> size_t
    {
        return (qwIndex + 1) & QUEUE_MASK;
    }

  public:
    auto Queue(rockEvent_t &&cEvent) -> void override
    {
        if (Next(m_qwTail) == m_qwHead)
        {
            m_qwHead = Next(m_qwHead);
        }

        m_ncEvents[m_qwTail] = std::move(cEvent);
        m_qwTail = Next(m_qwTail);
    }

    auto Poll(rockEvent_t &cOutEvent) -> bool override
    {
        if (m_qwHead == m_qwTail)
        {
            return false;
        }

        cOutEvent = m_ncEvents[m_qwHead];
        m_qwHead = Next(m_qwHead);
        return true;
    }
};

//******************************************************************************

auto GetEventQueue() -> EventQueue &
{
    static EventQueueImpl platform;
    return platform;
}

} // namespace rock3d
