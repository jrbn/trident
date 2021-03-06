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


#ifndef STORAGESTRAT_H_
#define STORAGESTRAT_H_

#include <trident/binarytables/binarytableinserter.h>
#include <trident/binarytables/rowtableinserter.h>
#include <trident/binarytables/columntableinserter.h>
#include <trident/binarytables/clustertableinserter.h>

#include <trident/binarytables/newcolumntable.h>
#include <trident/binarytables/newcolumntableinserter.h>
#include <trident/binarytables/newrowtable.h>
#include <trident/binarytables/binarytablereaders.h>
#include <trident/binarytables/newrowtableinserter.h>
#include <trident/binarytables/newclustertableinserter.h>
#include <trident/binarytables/factorytables.h>

#include <trident/kb/consts.h>

#include <kognac/factory.h>

/*
 * Old format: 2bits <storage format> -- 1bit <delta on the first term> -- 2 bit <compr on firs el> -- 2 bit <compr on second el> -- 1 bit <aggregated>
 */

/*
 * Current format: 3bits <storage format> -- 1bit <delta on the first term> -- 1 bit <compr on firs el> -- 1 bit <compr on second el> -- 1 bit <aggregated> -- 1 bit unused
 */

#define RATE_LIST 1.05

struct Statistics {
    long nListStrategies;
    long nList2Strategies;
    long nGroupStrategies;

    long nFirstCompr1;
    long nFirstCompr2;
    long nSecondCompr1;
    long nSecondCompr2;

    long diff;
    long nodiff;

    long exact;
    long approximate;

    long aggregated, notAggregated;

    Statistics() {
        nList2Strategies = nListStrategies = nGroupStrategies = 0;
        nFirstCompr1 = nFirstCompr2 = nSecondCompr1 = nSecondCompr2 = 0;
        exact = approximate = 0;
        diff = nodiff  = 0;
        aggregated = 0;
        notAggregated = 0;
    }
};

class StorageStrat {
public:
    struct Combinations {
        int type;
        int compr1Mode;
        int compr2Mode;
        int diffMode;
        long sum;
    };

protected:
    static unsigned getStrat1();
    static unsigned getStrat2();
    static unsigned getStrat3();
    static unsigned getStrat4();
    static unsigned getStrat5();
    static unsigned getStrat6();
    static unsigned getStrat7();

    static void createAllCombinations(std::vector<Combinations> &output,
                                      long *groupCounters1Compr2,
                                      long *listCounters1Compr2,
                                      long groupCounters2Compr2,
                                      long listCounters2Compr2,
                                      long *groupCounters1Compr1,
                                      long *listCounters1Compr1,
                                      long groupCounters2Compr1,
                                      long listCounters2Compr1);

private:
    long static minsum(const long counters1[2][2], const long counters2[2], int &c1, int &c2, int &d);

    unsigned static setCompr1(const unsigned signature, unsigned compr) {
        return (compr == COMPR_2 ? signature | 0x8 : signature & 0xF7);
    }

    unsigned static getCompr1(const unsigned signature) {
        return ((signature & 0x8) != 0) ? COMPR_2 : COMPR_1;
    }

    unsigned static setCompr2(const unsigned signature, int compr) {
        return (compr == COMPR_2 ? signature | 0x4 : signature & 0xF7);
    }

    unsigned static getCompr2(const unsigned signature) {
        return ((signature & 0x4) != 0) ? COMPR_2 : COMPR_1;
    }

    unsigned static setDiff1(const unsigned signature, int diff) {
        return (diff == NO_DIFFERENCE ? signature | 0x10 : signature & 0xEF);
    }

    unsigned static getDiff1(const unsigned signature) {
        return ((signature & 0x10) != 0) ? NO_DIFFERENCE : DIFFERENCE;
    }

    unsigned static setAggregated(const unsigned signature, const bool aggregate) {
        if (aggregate) {
            return signature | 1;
        } else {
            return signature & 0xFE;
        }
    }

    /*Factory<RowTable> *f1;
    Factory<ClusterTable> *f2;
    Factory<ColumnTable> *f3;*/
    Factory<NewColumnTable> *f4;
    FactoryNewRowTable *f5;
    FactoryNewClusterTable *f6;

    Factory<RowTableInserter> *f1i;
    Factory<ClusterTableInserter> *f2i;
    Factory<ColumnTableInserter> *f3i;
    Factory<NewColumnTableInserter> *f4i;
    Factory<NewRowTableInserter> *f5i;
    Factory<NewClusterTableInserter> *f6i;

public:
    /*static const unsigned FIXEDSTRAT1;
    static const unsigned FIXEDSTRAT2;
    static const unsigned FIXEDSTRAT3;
    static const unsigned FIXEDSTRAT4;*/
    static const unsigned FIXEDSTRAT5;
    static const unsigned FIXEDSTRAT6;
    static const unsigned FIXEDSTRAT7;

    bool static isAggregated(const char signature) {
        unsigned type = getStorageType(signature);
        return (signature & 1) && type != NEWCLUSTER_ITR;
    }

    unsigned static setStorageType(const unsigned signature, int type) {
        return signature | type << 5;
    }

    unsigned static setBytesField1(const unsigned signature, int nbytes) {
        return signature | (nbytes << 3);
    }

    unsigned static setBytesField2(const unsigned signature, int nbytes) {
        return signature | (nbytes << 1);
    }

    long statsCluster, statsRow, statsColumn;

    static size_t getBinaryBreakingPoint();

    static bool determineAggregatedStrategy(long *v1, long *v2, const int size,
                                            const long nTerms, Statistics &stats);

    static char determineStrategy(long *v1, long *v2, const int size,
                                  const long nTerms,
                                  const size_t nTermsClusterColumn,
                                  const bool useRowForLargeTables,
                                  Statistics &stats);

    static char determineStrategyOld(long *v1, long *v2, const int size,
                                  const long nTerms,
                                  const size_t nTermsClusterColumn,
                                  Statistics &stats);

    static char getStrategy(int typeStorage, int diff, int compr1, int compr2,
                            bool aggregated);

    unsigned static getStorageType(const unsigned signature) {
        return signature >> 5 & 7;
    }

    StorageStrat() {
        /*f1 = NULL;
        f2 = NULL;
        f3 = NULL;*/
        f4 = NULL;
        f5 = NULL;
        f6 = NULL;
        statsCluster = statsRow = statsColumn = 0;
    }

    void resetCounters() {
        statsCluster = statsRow = statsColumn = 0;
    }

    void init(/*Factory<RowTable> *listFactory,
              Factory<ClusterTable> *comprFactory,
              Factory<ColumnTable> *list2Factory,*/
              Factory<NewColumnTable> *ncFactory,
              FactoryNewRowTable *newRowFactories,
              FactoryNewClusterTable *newClusterFactories,
              Factory<RowTableInserter> *listFactory_i,
              Factory<ClusterTableInserter> *comprFactory_i,
              Factory<ColumnTableInserter> *list2Factory_i,
              Factory<NewColumnTableInserter> *ncFactory_i,
              Factory<NewRowTableInserter> *nrFactory_i,
              Factory<NewClusterTableInserter> *ncluFactory_i) {
        /*this->f1 = listFactory;
        this->f2 = comprFactory;
        this->f3 = list2Factory;*/
        this->f4 = ncFactory;
        this->f5 = newRowFactories;
        this->f6 = newClusterFactories;
        this->f1i = listFactory_i;
        this->f2i = comprFactory_i;
        this->f3i = list2Factory_i;
        this->f4i = ncFactory_i;
        this->f5i = nrFactory_i;
        this->f6i = ncluFactory_i;
    }

    PairItr *getBinaryTable(const char signature);

    BinaryTableInserter *getBinaryTableInserter(const char signature);
};

#endif
