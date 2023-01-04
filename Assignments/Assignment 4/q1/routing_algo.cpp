#include "node.h"
#include <iostream>

using namespace std;

void printRT(vector<RoutingNode *> nd)
{
    /*Print routing table entries*/
    for (int i = 0; i < nd.size(); i++)
    {
        nd[i]->printTable();
    }
}

void routingAlgo(vector<RoutingNode *> nd)
{
    //Your code here
    bool saturation = true;
    bool isChanged;
    vector<struct routingtbl> routingT;

    for(int i=1; i<nd.size(); ++i) {
    for (RoutingNode* node: nd){
      node->sendMsg();
    }
  }

    while(saturation)
    {
        routingT.clear();
        isChanged = false;
        for (auto i : nd)
        {
            routingT.push_back(i->getTable());
            i->sendMsg();
        }
        
        int sizeOFnd = nd.size();

        for (int i = 0; i < sizeOFnd; i++)
        {
            if (routingT[i].tbl.size() != (nd[i]->getTable()).tbl.size())
            {
                isChanged = true;
                break;
            }
            else
            {
            int sizeOFParticluarND = (nd[i]->getTable()).tbl.size();
            for (int j = 0; j < sizeOFParticluarND; j++)
            {
                string tableNExtHop = routingT[i].tbl[j].nexthop;
                string ndTbaleNExthop = nd[i]->getTable().tbl[j].nexthop;
                if (tableNExtHop != ndTbaleNExthop)
                {
                    isChanged = true;
                    break;
                }
                else
                {
                    string routinfInter = routingT[i].tbl[j].ip_interface;
                    string ndInter = (nd[i]->getTable()).tbl[j].ip_interface;
                    if (routinfInter == ndInter)
                    {
                    }
                    else
                    {
                        isChanged = true;
                        break;
                    }
                }
            }
            }
        }
        if (!isChanged)
        {
            saturation = false;
        }
    } 
    /*Print routing table entries after routing algo converges */
    printRT(nd);
}
int findMinimum(vector<RoutingNode*> nd,int visited[],int size)
{
  int minVertex =-1;
  for(int i =0;i<size;i++)
  {
    if(!visited && (minVertex==-1 || nd[i] < nd[minVertex]))
          minVertex = i;
  }
  return minVertex;
}
void RoutingNode::recvMsg(RouteMsg *msg)
{
    //your code here
    // here it doing till sender table size I mean how many ways it connected or go aisa hi samaj aa raha hai
    for (int i = 0; i < msg->mytbl->tbl.size(); i++)
    {
        RoutingEntry curr = msg->mytbl->tbl[i];
        curr.nexthop = msg->from;
        curr.ip_interface = msg->recvip;
        
        // If the edge is infinite or negative we did 0 cuz dijesktra don't count for negative edgess
        if(curr.cost<0)
            curr.cost=0;
        else if (msg->from == curr.dstip)
        {
            curr.cost = 1;
        }
        else
        {
            curr.cost = msg->mytbl->tbl[i].cost;
            curr.cost += 1;
        }
        bool isPresent = false;
        int destination = mytbl.tbl.size();
        for (int j = 0; j < destination; j++)
        {
            string Currently = msg->mytbl->tbl[i].dstip;
            string next = mytbl.tbl[j].dstip;

            if(Currently!= next)
            {
                continue;
            }
            else
           {
            if (Currently == next)
            {
                isPresent = true;
                mytbl.tbl[j].cost = min(mytbl.tbl[j].cost, msg->mytbl->tbl[i].cost + 1);
                break;
            }
            }
        }
        if (!isPresent)
        {
            mytbl.tbl.push_back(curr);
        }
    }
    routingtbl *recvRoutingTable = msg->mytbl;
  for (RoutingEntry entry : recvRoutingTable->tbl) {
    // Check routing entry

    bool entryExists = false;
    for ( int i=0; i<mytbl.tbl.size(); ++i) {
      RoutingEntry myEntry = mytbl.tbl[i];
      //printf("i=%d, nodeRT.cost=%d, DV.cost=%d\n",i, myEntry.cost, entry.cost );
      if (myEntry.dstip==entry.dstip){
        entryExists=true;
        //update existing entry
        if (myEntry.cost>entry.cost+1) {
          myEntry.cost=entry.cost+1;
          myEntry.nexthop=msg->from;
          mytbl.tbl[i]=myEntry;
        }
      }
    }
    if (!entryExists) {
      // add the new entry
      RoutingEntry newEntry;
      newEntry.dstip = entry.dstip;
      newEntry.nexthop = msg->from;
      newEntry.ip_interface = msg->recvip;
      newEntry.cost = entry.cost+1;
      mytbl.tbl.push_back(newEntry);
    }
  }
 
}
