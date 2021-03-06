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


#ifndef CACHE_H_
#define CACHE_H_

#include <trident/tree/nodemanager.h>
#include <trident/tree/node.h>
#include <trident/tree/leaffactory.h>
#include <trident/kb/consts.h>

#include <boost/chrono.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/log/trivial.hpp>

#include <list>
#include <string>

class Node;
class TreeContext;

namespace timens = boost::chrono;

class Cache {

private:
    TreeContext *context;

    const bool compressedNodes;

    boost::circular_buffer<Node*> registeredNodes;

    NodeManager *manager;

    char supportBuffer[SIZE_SUPPORT_BUFFER];
    char supportBuffer2[SIZE_SUPPORT_BUFFER];

    LeafFactory *factory;
public:

    Cache(int maxNodesInCache, bool compressedNodes) :
        compressedNodes(compressedNodes), registeredNodes(maxNodesInCache) {
//      BOOST_LOG_TRIVIAL(debug)<< "Init cache: maxNodesInCache=" << maxNodesInCache << " compressed? " << compressedNodes;
        context = NULL;
        factory = NULL;
        manager = NULL;
        memset(supportBuffer, 0, SIZE_SUPPORT_BUFFER);
        memset(supportBuffer2, 0, SIZE_SUPPORT_BUFFER);
    }

    void init(TreeContext *context, std::string path, int fileMaxSize,
              int maxNFiles, long cacheMaxSize, int sizeLeavesFactory,
              int sizePreallLeavesFactory, int nodeMinBytes);

    Node *getNodeFromCache(long id);

    void registerNode(Node *node);

    Leaf *newLeaf() {
        return factory->get();
    }

    void releaseLeaf(Node *leaf) {
        factory->release((Leaf*)leaf);
    }

    IntermediateNode *newIntermediateNode();

    IntermediateNode *newIntermediateNode(Node *child1, Node *child2);

    void flushNode(Node *node, const bool registerNode);

    void flushAllCache();

    ~Cache() {
        Node *node = NULL;
        while (!registeredNodes.empty()) {
            node = registeredNodes.front();
            registeredNodes.pop_front();
            if (node->shouldDeallocate())
                delete node;
        }
        registeredNodes.clear();

        delete factory;
        delete manager;
    }
};

#endif /* CACHE_H_ */
