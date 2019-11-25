#ifndef VERSION_H
#define VERSION_H

//#pragma once

#define DRIVER_VER_MAJOR 0
#define DRIVER_VER_MINOR 1
#define DRIVER_VER_BUILD 2
#define DRIVER_VER_PATCH 0

#define DRIVER_VERSION (DRIVER_VER_MAJOR * 10000 + DRIVER_VER_MINOR * 100 + DRIVER_VER_PATCH)

/*
 * \todo
 * 1. to overload ostream for printing Data structure
 * 2. to make translation of time_point to string
 * 3. configuration throw file like xml
 * 4. we must have possibility to change ID device
 * 5. thread-safe logging
 * 6. thread-safe transfering data when reading
 * 7. to make PLC simulator (libmodbus interface)
 * 8. integrate version into project
 * 9. add debug version of libmodbus library
 * 10. to think through liccense
 *
 * \bag
 * 1. When is set range {0,5} result range {0,10} if not connection. Check!!!
 */

#endif // VERSION_H

