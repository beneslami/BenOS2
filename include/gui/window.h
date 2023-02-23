#ifndef WINDOW_H
#define WINDOW_H

namespace BenOS{
    namespace gui{
        class Window : public CompositeWidget{
        protected:
            bool Dragging;
        public:
            Window();
        };
    }
}

#endif