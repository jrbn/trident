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


#ifndef _RM_COMPOSITE_TERM_ITR_H
#define _RM_COMPOSITE_TERM_ITR_H

#include <trident/iterators/pairitr.h>
#include <trident/iterators/difftermitr.h>
#include <trident/kb/consts.h>

class RmCompositeTermItr : public PairItr {
private:
    PairItr *mainitr;
    PairItr *rmitr;
    long currentCount, nextCount;

    bool hnc, hn;

    static bool _sorter(PairItr *i1, PairItr *i2) {
        return i1->getKey() > i2->getKey();
    }
public:
    int getTypeItr() {
        return RMCOMPOSITETERM_ITR;
    }

    PairItr *getMainItr() {
        return mainitr;
    }

    PairItr *getRmItr() {
        return rmitr;
    }

    long getValue1() {
        throw 10; //not supported
    }

    long getValue2() {
        throw 10; //not supported
    }

    void ignoreSecondColumn() {
        throw 10; //not supported
    }

    void clear() {
    }

    void mark() {
        throw 10;
    }

    void reset(const char i) {
        throw 10;
    }

    void moveto(const long c1, const long c2) {
        throw 10;
    }

    long getCount() {
        return currentCount;
    }

    bool hasNext() {
        if (hnc)
            return hn;

        hn = false;
        if (mainitr->hasNext()) {
            mainitr->next();
            while (true) {
                while (rmitr->getKey() < mainitr->getKey() && rmitr->hasNext()) {
                    rmitr->next();
                }
                if (rmitr->getKey() != mainitr->getKey()) {
                    nextCount = mainitr->getCount();
                    hn = true;
                    break;
                } else {
                    //The two are the same. What about the count?
                    size_t rmcount = rmitr->getCount();
                    assert(rmcount != 0);
                    size_t maincount = mainitr->getCount();
                    assert(maincount != 0);
                    if (rmcount == maincount) {
                        //Move to the next key
                        if (mainitr->hasNext()) {
                            mainitr->next();
                        } else {
                            break;
                        }
                    } else {
                        nextCount = mainitr->getCount() - rmitr->getCount();
                        hn = true;
                        break;
                    }
                }
            }
        }
        hnc = true;
        return hn;
    }

    uint64_t getCardinality() {
        return mainitr->getCardinality() - rmitr->getCardinality();
    }

    uint64_t estCardinality() {
        return getCardinality();
    }

    void next() {
        if (!hnc && !hasNext()) {
            return;
        }
        setKey(mainitr->getKey());
        currentCount = nextCount;
        hnc = false;
    }

    void init(PairItr * mainitr, PairItr * rmitr) {
	initializeConstraints();
        this->mainitr = mainitr;
        this->rmitr = rmitr;
        this->currentCount = 0;
        if (rmitr->hasNext())
            rmitr->next();
        hnc = false;
        hn = false;
    }
};

#endif
