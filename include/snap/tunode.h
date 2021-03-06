#ifndef _TUNODE_H
#define _TUNODE_H

#include <snap/readers.h>

class TUNode {
    private:
        //key (5 bytes), nelements (5bytes) strat (1 byte), file (2 bytes), pos (5 bytes)
        const char *rawblock;

    public:
        TUNode() : rawblock(NULL) {}

        TUNode(const TUNode *node) {
            this->rawblock = node->rawblock;
        }

        TUNode& operator++ (int) {
            rawblock += 18;
            return *this;
        }

        bool operator < (const TUNode& Node) const {
            return rawblock < Node.rawblock;
        }

        bool operator == (const TUNode& Node) const {
            return Node.rawblock == rawblock;
        }

        TUNode(const char *rawblock) : rawblock(rawblock) {
        }

        /// Returns ID of the current node.
        long GetId() const {
            const long v = (*(long*)(rawblock)) & 0XFFFFFFFFFFl;
            return v;
        }

        /// Returns degree of the current node, since the in-degree and out-degree are equal, I just pick one
        long GetDeg() const {
            //const long v1 = (*(long*)(rawblock + 18)) & 0XFFFFFFFFFFl;
            const long v2 = (*(long*)(rawblock + 5)) & 0XFFFFFFFFFFl;
            //return v1 + v2;
            return v2;
        }

        /// Returns in-degree of the current node.
        long GetInDeg() const {
            return GetDeg();
        }

        /// Returns out-degree of the current node.
        long GetOutDeg() const {
            return GetDeg();
        }

        /// Returns ID of NodeN-th out-node (the node the current node points to). ##TNGraph::TNodeI::GetOutNId
        long GetOutNId(const long& NodeN) const {
            const uint8_t strat = rawblock[10];
            SnapReaders::pReader reader = SnapReaders::readers[strat];
            const short file = *(short*)(rawblock + 11);
            const char *p = SnapReaders::f_sop[file];
            const long pos = (*(long*)(rawblock + 13)) & 0XFFFFFFFFFFl;
            const char *sop = p + pos;
            return reader(sop, NodeN);
        }

        /// Returns ID of NodeN-th in-node (the node pointing to the current node). ##TNGraph::TNodeI::GetInNId
        long GetInNId(const long& NodeN) const {
            return GetOutNId(NodeN);
        }

        /// Returns ID of NodeN-th neighboring node. ##TNGraph::TNodeI::GetNbrNId
        long GetNbrNId(const long& NodeN) const {
            const uint8_t strat = rawblock[10];
            SnapReaders::pReader reader = SnapReaders::readers[strat];
            const short file = *(short*)(rawblock + 11);
            const char *p = SnapReaders::f_sop[file];
            const long pos = (*(long*)(rawblock + 13)) & 0XFFFFFFFFFFl;
            const char *sop = p + pos;
            return reader(sop , NodeN);
        }

        SnapReaders::pReader GetOutReader() const {
            const uint8_t strat = rawblock[10];
            return SnapReaders::readers[strat];
        }

        int GetOutLenField() const {
            const uint8_t strat = rawblock[10];
            const char nbytes1 = (strat >> 3) & 3;
            switch (nbytes1) {
                case 0:
                    return 1;
                case 1:
                    return 2;
                case 2:
                    return 4;
                case 3:
                    return 5;
                default:
                    throw 10;
            }
        }

        int GetInLenField() const {
            return GetOutLenField();
        }

        const char *GetBeginOut() const {
            const short file = *(short*)(rawblock + 11);
            const char *p = SnapReaders::f_sop[file];
            const long pos = (*(long*)(rawblock + 13)) & 0XFFFFFFFFFFl;
            const char *sop = p + pos;
            return sop;
        }

        const char *GetBeginIn() const {
            return GetBeginOut();
        }

        /// Tests whether node with ID NId points to the current node.
        bool IsInNId(const long& NId) const {
            throw 10; //not defined for undirected graph
        }

        /// Tests whether the current node points to node with ID NId.
        bool IsOutNId(const long& NId) const {
            throw 10; //not defined for undirected graph
        }

        /// Tests whether node with ID NId is a neighbor of the current node.
        bool IsNbrNId(const long& NId) const {
            throw 10; //not implemented -- but should be
        }
};


#endif
