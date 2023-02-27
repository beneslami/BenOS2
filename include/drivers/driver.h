#ifndef DRIVER_H
#define DRIVER_H

namespace BenOS {
    namespace drivers {
        class Driver {
        public:
            Driver();
            ~Driver();
            virtual void Activate();
            virtual int Reset();
            virtual void Deactivate();
        };

        class DriverManager {
        public:
            Driver *drivers[255];
            int numDrivers;
            DriverManager();
            void AddDriver(Driver *);
            void ActivateAll();
        };
    }
}
#endif