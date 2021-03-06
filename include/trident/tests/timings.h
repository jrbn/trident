/*
 * Copyright 2017 Jacopo Urbani
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
**/


#ifndef _TIMINGS_H
#define _TIMINGS_H

#include <boost/chrono.hpp>
#include <string>

#define T_SPO 0
#define T_OPS 1
#define T_POS 2
#define T_SOP 3
#define T_OSP 4
#define T_PSO 5

class Timings {
private:
    std::string queryfile;

public:
    Timings(std::string filequeries) : queryfile(filequeries) {}

    virtual void init() = 0;

    virtual boost::chrono::duration<double> launchQuery(const int perm,
            const long s,
            const long p,
            const long o,
            const int countIgnores,
            long &c,
            long &junk) = 0;

    void launchTests();
};

#endif
