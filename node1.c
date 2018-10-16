#include <stdio.h>
#include "project3.h"

extern int TraceLevel;
extern float clocktime;
extern struct RoutePacket;
extern void toLayer2( struct RoutePacket packet );

struct distance_table {
  int costs[MAX_NODES][MAX_NODES];
};
struct distance_table dt1;
struct NeighborCosts   *neighbor1;

/* students to write the following two routines, and maybe some others */

void rtinit1() {
    // Output trace
    printf("At time t=%f, rtinit1() called.\n", clocktime);

    // Initialize distance table with "infinity"
    int i;
    for (i = 0; i < MAX_NODES; i++) {
        int j;
        for (j = 0; j < MAX_NODES; j++) {
            dt1.costs[i][j] = 9999;
        }
    }
    dt1.costs[1][1] = 0;

    // Store neighbor costs in distance table
    neighbor1 = getNeighborCosts(1);
    printf("At time t=%f, node 1 initial distance vector: ", clocktime);

    for (i = 0; i < neighbor1->NodesInNetwork; i++) {
        dt1.costs[i][i] = neighbor1->NodeCosts[i];
        printf("%d ", neighbor1->NodeCosts[i]);
    }
    printf("\n");

    // Print distance table
    printdt1(1, neighbor1, &dt1);

    // Send neighbors minimum cost paths to all other nodes
    for (i = 0; i < neighbor1->NodesInNetwork; i++) {
        if (i != 1) {
            // Output trace
            printf("At time t=%f, node 1 sends packet to node %d with: ", clocktime, i);

            struct RoutePacket packet;
            packet.sourceid = 1;
            packet.destid = i;
            int j;
            for (j = 0; j < neighbor1->NodesInNetwork; j++) {
                printf("%d ", neighbor1->NodeCosts[j]);
                packet.mincost[j] = neighbor1->NodeCosts[j];
            }
            printf("\n");

            toLayer2(packet);
        }
    }
}


void rtupdate1( struct RoutePacket *rcvdpkt ) {
    // Output trace
    printf("At time t=%f, rtupdate1() called.\n", clocktime);

    // Update distance table with rcvdpkt and algorithm
    int isChange = 0;
    int i;
    for (i = 0; i < neighbor1->NodesInNetwork; i++) {
        // Store other node distances
        //dt0.costs[rcvdpkt->sourceid][i] = rcvdpkt->mincost[i];
        // printf("%d ", neighbor0->NodeCosts[i]);
        int altWeight = rcvdpkt->mincost[i] + dt1.costs[rcvdpkt->sourceid][rcvdpkt->sourceid];
        if (dt1.costs[i][i] > altWeight) {
            isChange = 1;
            dt1.costs[i][i] = altWeight;
        }
    }

    printf("At time t=%f, node 1 current distance vector: ", clocktime);
    for (i = 0; i < neighbor1->NodesInNetwork; i++) {
        printf("%d ", dt1.costs[i][i]);
    }
    printf("\n");

    // Print distance table
    // TODO printdt1(1, neighbor1, &dt1);

    // If something changed, inform neighbors with new minimum path costs
    if (isChange) {

        // Send neighbors minimum cost paths to all other nodes
        for (i = 0; i < neighbor1->NodesInNetwork; i++) {
            if (i != 1) {
                // Output trace
                printf("At time t=%f, node 1 sends packet to node %d with: ", clocktime, i);

                struct RoutePacket packet;
                packet.sourceid = 1;
                packet.destid = i;
                int j;
                for (j = 0; j < neighbor1->NodesInNetwork; j++) {
                    printf("%d ", dt1.costs[j][j]);
                    packet.mincost[j] = dt1.costs[j][j];
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
void printdt1( int MyNodeNumber, struct NeighborCosts *neighbor,
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
}    // End of printdt1
