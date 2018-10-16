#include <stdio.h>
#include "project3.h"

extern int TraceLevel;
extern float clocktime;
extern struct RoutePacket;
extern void toLayer2( struct RoutePacket packet );

struct distance_table {
  int costs[MAX_NODES][MAX_NODES];
};
struct distance_table dt3;
struct NeighborCosts   *neighbor3;

/* students to write the following two routines, and maybe some others */

void rtinit3() {
    // Output trace
    printf("At time t=%f, rtinit3() called.\n", clocktime);

    // Initialize distance table with "infinity"
    int i;
    for (i = 0; i < MAX_NODES; i++) {
        int j;
        for (j = 0; j < MAX_NODES; j++) {
            dt3.costs[i][j] = 9999;
        }
    }
    dt3.costs[3][3] = 0;

    // Store neighbor costs in distance table
    neighbor3 = getNeighborCosts(3);
    printf("At time t=%f, node 3 initial distance vector: ", clocktime);

    for (i = 0; i < neighbor3->NodesInNetwork; i++) {
        dt3.costs[i][i] = neighbor3->NodeCosts[i];
        printf("%d ", neighbor3->NodeCosts[i]);
    }
    printf("\n");

    // Print distance table
    printdt3(3, neighbor3, &dt3);

    // Send neighbors minimum cost paths to all other nodes
    for (i = 0; i < neighbor3->NodesInNetwork; i++) {
        if (i != 3) {
            // Output trace
            printf("At time t=%f, node 3 sends packet to node %d with: ", clocktime, i);

            struct RoutePacket packet;
            packet.sourceid = 3;
            packet.destid = i;
            int j;
            for (j = 0; j < neighbor3->NodesInNetwork; j++) {
                printf("%d ", neighbor3->NodeCosts[j]);
                packet.mincost[j] = neighbor3->NodeCosts[j];
            }
            printf("\n");

            toLayer2(packet);
        }
    }
}


void rtupdate3( struct RoutePacket *rcvdpkt ) {
    // Output trace
    printf("At time t=%f, rtupdate3() called.\n", clocktime);

    // Update distance table with rcvdpkt and algorithm
    int isChange = 0;
    int i;
    for (i = 0; i < neighbor3->NodesInNetwork; i++) {
        // Store other node distances
        //dt0.costs[rcvdpkt->sourceid][i] = rcvdpkt->mincost[i];
        // printf("%d ", neighbor0->NodeCosts[i]);
        int altWeight = rcvdpkt->mincost[i] + dt3.costs[rcvdpkt->sourceid][rcvdpkt->sourceid];
        if (dt3.costs[i][i] > altWeight) {
            isChange = 1;
            dt3.costs[i][i] = altWeight;
        }
    }

    printf("At time t=%f, node 3 current distance vector: ", clocktime);
    for (i = 0; i < neighbor3->NodesInNetwork; i++) {
        printf("%d ", dt3.costs[i][i]);
    }
    printf("\n");

    // Print distance table
    // TODO printdt3(3, neighbor3, &dt3);

    // If something changed, inform neighbors with new minimum path costs
    if (isChange) {
        for (i = 0; i < neighbor3->NodesInNetwork; i++) {
            if (i != 3) {
                // Output trace
                printf("At time t=%f, node 3 sends packet to node %d with: ", clocktime, i);

                struct RoutePacket packet;
                packet.sourceid = 3;
                packet.destid = i;
                int j;
                for (j = 0; j < neighbor3->NodesInNetwork; j++) {
                    printf("%d ", dt3.costs[j][j]);
                    packet.mincost[j] = dt3.costs[j][j];
                }
                printf("\n");

                toLayer2(packet);
            }
        }
    }
}


/////////////////////////////////////////////////////////////////////
//  printdt
//  This routine is being supplied to you.  It is the same code in
//  each node and is tailored based on the input arguments.
//  Required arguments:
//  MyNodeNumber:  This routine assumes that you know your node
//                 number and supply it when making this call.
//  struct NeighborCosts *neighbor:  A pointer to the structure
//                 that's supplied via a call to getNeighborCosts().
//                 It tells this print routine the configuration
//                 of nodes surrounding the node we're working on.
//  struct distance_table *dtptr: This is the running record of the
//                 current costs as seen by this node.  It is
//                 constantly updated as the node gets new
//                 messages from other nodes.
/////////////////////////////////////////////////////////////////////
void printdt3( int MyNodeNumber, struct NeighborCosts *neighbor,
		struct distance_table *dtptr ) {
    int       i, j;
    int       TotalNodes = neighbor->NodesInNetwork;     // Total nodes in network
    int       NumberOfNeighbors = 0;                     // How many neighbors
    int       Neighbors[MAX_NODES];                      // Who are the neighbors

    // Determine our neighbors
    for ( i = 0; i < TotalNodes; i++ )  {
        if (( neighbor->NodeCosts[i] != INFINITY ) && i != MyNodeNumber )  {
            Neighbors[NumberOfNeighbors] = i;
            NumberOfNeighbors++;
        }
    }
    // Print the header
    printf("                via     \n");
    printf("   D%d |", MyNodeNumber );
    for ( i = 0; i < NumberOfNeighbors; i++ )
        printf("     %d", Neighbors[i]);
    printf("\n");
    printf("  ----|-------------------------------\n");

    // For each node, print the cost by travelling thru each of our neighbors
    for ( i = 0; i < TotalNodes; i++ )   {
        if ( i != MyNodeNumber )  {
            printf("dest %d|", i );
            for ( j = 0; j < NumberOfNeighbors; j++ )  {
                    printf( "  %4d", dtptr->costs[i][Neighbors[j]] );
            }
            printf("\n");
        }
    }
    printf("\n");
}    // End of printdt3
