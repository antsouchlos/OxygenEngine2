#ifndef OE_EVENT_H
#define OE_EVENT_H

#include <OE/Events/event_parser.h>
#include <OE/task.h>
#include <atomic>
#include <functional>
#include <memory>
#include <set>
#include <unordered_set>

/** Temporary documentation: (OUTDATED)
 *
 * event_t::call(OE_Task*):
 * Return values:
 *  0: successfully run
 *  1: event is executed in another thread
 *  2++ or -1--: papares gia tin ora (add your own if necessary)
 * ------------------------------------
 * Insert custom internal event type here
 * This is needed to be able to do the correct casts afterwards
 * Andreas' TODO:
 * -Collision Events
 * -Network Events
 * -Anything else his subsystems need
 * Philsegeler's TODO:
 * -Anything else his subsystems need
 */
namespace oe {

    enum event_type {
        CUSTOM_EVENT     = 0,
        KEYBOARD_EVENT   = 1,
        MOUSE_EVENT      = 2,
        GAMEPAD_EVENT    = 3,
        NETWORK_EVENT    = 4,
        PHYSICS_EVENT    = 5,
        MOUSE_MOVE_EVENT = 6,
        EVENT_COMBO      = 7
    };

    task_action template_event_func(const task_info_t);

    /* general event type */
    class event_t : public OE_THREAD_SAFETY_OBJECT {
        friend class event_handler_t;
        friend class input_event_handler_t;

    public:
        // static bool finished;
        event_t();
        virtual ~event_t();
        virtual task_action call();

        static std::atomic<std::size_t> current_id;
        const std::size_t               id;

    protected:
        // internal_call() is implemented in OE_Error.cpp
        task_action internal_call();

        void set_func(const method_type);

        std::atomic<bool> active_{false};

        event_type  type_;
        method_type func_{&template_event_func};

        task_info_t task_;

        bool                            has_init_{false};
        std::unordered_set<std::size_t> sub_events_;
    };

    /*button event used in keyboard/mouse/gamepad*/


    enum button_type { BUTTON_RELEASE = 0, BUTTON_PRESS = 2, BUTTON_JUST_PRESS = 1, BUTTON_JUST_RELEASE = 3 };

    /// class intended to store keyboard events (3 for each )
    class keyboard_event_t : public event_t {
        friend class event_handler_t;
        friend class input_event_handler_t;

    public:
        keyboard_event_t();
        ~keyboard_event_t();
        task_action call();

    private:
        uint8_t     keystate_;
        std::string key_;
        bool        is_main_event_{false};
    };

    /// class intended to store mouse events (3 for each mouse buttons 1-5)
    class mouse_event_t : public event_t {
        friend class event_handler_t;
        friend class input_event_handler_t;

    public:
        mouse_event_t();
        ~mouse_event_t();
        task_action call();

    private:
        uint8_t     keystate_;
        bool        is_main_event_{false};
        std::string key_;
    };

    /// class intended to store mouse move events
    class mouse_move_event_t : public event_t {
        friend class event_handler_t;
        friend class input_event_handler_t;

    public:
        mouse_move_event_t();
        ~mouse_move_event_t();
        task_action call();
    };

    /// class intended to store gamepad events (3 for each mouse buttons 1-5 and mouse position)
    class gamepad_event_t : public event_t {
        friend class event_handler_t;
        friend class input_event_handler_t;

    public:
        gamepad_event_t();
        ~gamepad_event_t();
        task_action call();

    private:
        uint8_t     keystate_;
        std::string key_;

        int  axis_;
        bool axismoved_;
        int  x_, y_, delta_x_, delta_y_;
    };

    /// class intended for user events
    class custom_event_t : public event_t {
        friend class event_handler_t;
        friend class input_event_handler_t;

    public:
        custom_event_t();
        ~custom_event_t();

    protected:
    };

    /// class intended for physics events
    class renderer_event_t : public event_t {
        friend class event_handler_t;
        friend class input_event_handler_t;

    public:
        renderer_event_t()  = default;
        ~renderer_event_t() = default;

    protected:
    };

    /// class intended for physics events
    class physics_event_t : public event_t {
        friend class event_handler_t;
        friend class input_event_handler_t;

    public:
        physics_event_t();
        ~physics_event_t();

    protected:
    };

    /// class intended for network events
    class network_event_t : public event_t {
        friend class event_handler_t;
        friend class input_event_handler_t;

    public:
        network_event_t();
        ~network_event_t();

    protected:
    };

};     // namespace oe
#endif // OE_EVENT_H
