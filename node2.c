#include <stdio.h>
#include "project3.h"

extern int TraceLevel;
extern float clocktime;
extern struct RoutePacket;
extern void toLayer2( struct RoutePacket packet );

struct distance_table {
  int costs[MAX_NODES][MAX_NODES];
};
struct distance_table dt2;
struct NeighborCosts   *neighbor2;

/* students to write the following two routines, and maybe some others */

void rtinit2() {
    // Output trace
    printf("At time t=%f, rtinit2() called.\n", clocktime);

    // Initialize distance table with "infinity"
    int i;
    for (i = 0; i < MAX_NODES; i++) {
        int j;
        for (j = 0; j < MAX_NODES; j++) {
            dt2.costs[i][j] = 9999;
        }
    }
    dt2.costs[2][2] = 0;

    // Store neighbor costs in distance table
    neighbor2 = getNeighborCosts(2);
    printf("At time t=%f, node 2 initial distance vector: ", clocktime);

    for (i = 0; i < neighbor2->NodesInNetwork; i++) {
        dt2.costs[i][i] = neighbor2->NodeCosts[i];
        printf("%d ", neighbor2->NodeCosts[i]);
    }
    printf("\n");

    // Print distance table
    printdt2(2, neighbor2, &dt2);

    // Send neighbors minimum cost paths to all other nodes
    for (i = 0; i < neighbor2->NodesInNetwork; i++) {
        if (i != 2) {
            // Output trace
            printf("At time t=%f, node 2 sends packet to node %d with: ", clocktime, i);

            struct RoutePacket packet;
            packet.sourceid = 2;
            packet.destid = i;
            int j;
            for (j = 0; j < neighbor2->NodesInNetwork; j++) {
                printf("%d ", neighbor2->NodeCosts[j]);
                packet.mincost[j] = neighbor2->NodeCosts[j];
            }
            printf("\n");

            toLayer2(packet);
        }
    }
}


void rtupdate2( struct RoutePacket *rcvdpkt ) {
    // Output trace
    printf("At time t=%f, rtupdate2() called.\n", clocktime);

    // Update distance table with rcvdpkt and algorithm
    int isChange = 0;
    int i;
    for (i = 0; i < neighbor2->NodesInNetwork; i++) {
        // Store other node distances
        //dt0.costs[rcvdpkt->sourceid][i] = rcvdpkt->mincost[i];
        // printf("%d ", neighbor0->NodeCosts[i]);
        int altWeight = rcvdpkt->mincost[i] + dt2.costs[rcvdpkt->sourceid][rcvdpkt->sourceid];
        if (dt2.costs[i][i] > altWeight) {
            isChange = 1;
            dt2.costs[i][i] = altWeight;
        }
    }

    printf("At time t=%f, node 2 current distance vector: ", clocktime);
    for (i = 0; i < neighbor2->NodesInNetwork; i++) {
        printf("%d ", dt2.costs[i][i]);
    }
    printf("\n");

    // Print distance table
    // TODO printdt2(2, neighbor2, &dt2);

    // If something changed, inform neighbors with new minimum path costs
    if (isChange) {

        // Send neighbors minimum cost paths to all other nodes
        for (i = 0; i < neighbor2->NodesInNetwork; i++) {
            if (i != 2) {
                // Output trace
                printf("At time t=%f, node 2 sends packet to node %d with: ", clocktime, i);

                struct RoutePacket packet;
                packet.sourceid = 2;
                packet.destid = i;
                int j;
                for (j = 0; j < neighbor2->NodesInNetwork; j++) {
                    printf("%d ", dt2.costs[j][j]);
                    packet.mincost[j] = dt2.costs[j][j];
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
void printdt2( int MyNodeNumber, struct NeighborCosts *neighbor,
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
}    // End of printdt2
