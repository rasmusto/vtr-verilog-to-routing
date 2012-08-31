/*

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/
#include "container.h"
#include <time.h>

Container::Container(ExplorerScene* scene)
{
    myScene = scene;
    maxlayer = 0;
    maxcountPerLayer = 0;
    myOdin = new OdinInterface();
    odinStarted = false;
    simOffset = 63;
    maxSimStep = 64;
    actSimStep = 0;
}

/*---------------------------------------------------------------------------------------------
 * (function: setFilename)
 *-------------------------------------------------------------------------------------------*/
void Container::setFilename(QString filename)
{
    myFilename = filename;
    myOdin->setFilename(filename);
}

/*---------------------------------------------------------------------------------------------
 * (function: addUnit)
 *-------------------------------------------------------------------------------------------*/
LogicUnit * Container::addUnit(QString name,LogicUnit::UnitType type, QPointF position,
                               nnode_t* odinRef)
{
    LogicUnit* newUnit;
    //create in scene
    newUnit = myScene->addLogicUnit(name,type,position);
    newUnit->setPos(position);
    newUnit->setOdinRef(odinRef);
    unithashtable[name] = newUnit;
    unitcontainer.append(newUnit);
    return newUnit;
}

/*---------------------------------------------------------------------------------------------
 * (function: addConnectionHash)
 *-------------------------------------------------------------------------------------------*/
bool Container::addConnectionHash(QString start, QString end)
{
    //do not connect modules with themself
    if(start.compare(end) == 0)
        return false;

    bool startFound = false;
    bool endFound = false;
    bool success = false;
    LogicUnit *startUnit, *endUnit;
    if(unithashtable.contains(start)){
        startUnit = unithashtable.value(start);
        startFound = true;
    }
    if(unithashtable.contains(end)){
        endUnit = unithashtable.value(end);
        endFound = true;
    }
    if(startFound && endFound){
        myScene->addConnection(startUnit, endUnit);
        success = true;
    }
    return success;

}

/*---------------------------------------------------------------------------------------------
 * (function: arrangeContainer)
 *-------------------------------------------------------------------------------------------*/
//arrange the modules according to their layer.
//at the moment wire crossing is not considered
void Container::arrangeContainer()
{
    computeLayers();
    spreadLayers();
    myScene->setSceneRect(0,0,1000.0+400*maxlayer+maxcountPerLayer*50,1000.0+400*maxcountPerLayer);
}

/*---------------------------------------------------------------------------------------------
 * (function: deleteModule)
 *-------------------------------------------------------------------------------------------*/
void Container::deleteModule(QString name)
{
    for(int i = 0;i<unitcontainer.count();i++){
        LogicUnit *temp = unitcontainer.at(i);
        if(temp->getName().compare(name)==0){
            unitcontainer.removeAt(i);
        }
    }
    if(unithashtable.contains(name))
        unithashtable.remove(name);
}

/*---------------------------------------------------------------------------------------------
 * (function: clearContainer)
 *-------------------------------------------------------------------------------------------*/
//delete everything in the container
void Container::clearContainer()
{
    while(!unitcontainer.empty()){
        unitcontainer.removeFirst();
    }
    unithashtable.clear();
}

/*---------------------------------------------------------------------------------------------
 * (function: setLayer)
 *-------------------------------------------------------------------------------------------*/
void Container::setLayer(QString unitName, int layercount){
    QString actName;

    foreach (LogicUnit *actUnit, unitcontainer) {
        actName = actUnit->getName();
        if(actName.compare(unitName)==0){
            if(layercount > actUnit->getLayer()){
                   actUnit->setLayer(layercount);
            }
        }
    }

}

/*---------------------------------------------------------------------------------------------
 * (function: computeLayersHash)
 *-------------------------------------------------------------------------------------------*/
//A smarter function to compute the layers.
void Container::computeLayersHash()
{
    QHash<QString, LogicUnit *> currentStep = arrHashtable;
    QHash<QString, LogicUnit *> nextStep;
    QHash<QString, LogicUnit *>::const_iterator blockIterator = currentStep.constBegin();
    LogicUnit *actUnit;

    for(int i = unitcontainer.count(); i>=0;i--){
        while(blockIterator != currentStep.constEnd()){
            actUnit = blockIterator.value();
            //adjust the layers of all "children"
            QList<Wire *> cons = actUnit->getOutCons();
            foreach(Wire *wire, cons){
                //go through all of the ends and set the layer to layer+1
                int newLayer = actUnit->getLayer()+1;
                wire->endUnit()->setLayer(newLayer);
                nextStep.insert(wire->endUnit()->getName(),wire->endUnit());
                if(newLayer > maxlayer){
                    maxlayer = newLayer;
                }
            }
            //get next Item
            ++blockIterator;
        }
        currentStep.clear();
        currentStep = nextStep;
        nextStep.clear();
        blockIterator = currentStep.constBegin();
    }
}

/*---------------------------------------------------------------------------------------------
 * (function: getMaxParentLayer)
 *-------------------------------------------------------------------------------------------*/
int Container::getMaxParentLayer(LogicUnit* node){
    QList<LogicUnit*> parents = node->getParents();
    int result = -1;
    foreach(LogicUnit* parent, parents){
        if(parent->getLayer() > result){
            result = parent->getLayer();
        }
    }
    return result;
}

/*---------------------------------------------------------------------------------------------
 * (function: copySimCyclesintoNodes)
 *-------------------------------------------------------------------------------------------*/
void Container::copySimCyclesIntoNodes()
{   //for each cycle which was simulated         //int value;
    //int i;, copy the values into the nodes
    int cycle;
    for(cycle=0;cycle<maxSimStep;cycle++){
        QHash<QString, nnode_t *>::const_iterator blockIterator = odinTable.constBegin();

        while(blockIterator != odinTable.constEnd()){
             int value;
             int i;
             QString name = blockIterator.key();
             LogicUnit* visNode = unithashtable[name];
             nnode_t* node = blockIterator.value();
             //for each output pin of the node update the current value
             for(i=0;i<node->num_output_pins;i++){
                 value = myOdin->getOutputValue(node, i, cycle);
                 visNode->setOutValue(i, value, cycle);
                 visNode->updateWires();
             }
             ++blockIterator;
        }
    }
}

/*---------------------------------------------------------------------------------------------
 * (function: computeLayers)
 *-------------------------------------------------------------------------------------------*/
//compute the layers of all modules inside the container
void Container::computeLayers()
{
    int i;

    QQueue<LogicUnit*> nodequeue;
    QHash<QString, LogicUnit*> donehashtable;
    for (i = 0; i < verilog_netlist->num_top_input_nodes; i++){
        QString name = verilog_netlist->top_input_nodes[i]->name;
        nodequeue.enqueue(getReferenceToUnit(name));
    }

    // Enqueue constant nodes.
    nnode_t* constant_nodes[] = {verilog_netlist->gnd_node,
                                 verilog_netlist->vcc_node,
                                 verilog_netlist->pad_node};
    int num_constant_nodes = 3;
    for (i = 0; i < num_constant_nodes; i++){
        QString name = constant_nodes[i]->name;
        nodequeue.enqueue(getReferenceToUnit(name));
    }

    // go through the netlist. While doing so
    // remove nodes from the queue and add followup nodes
    LogicUnit* node;
    int maxparent;
    while(!nodequeue.isEmpty()){
        node = nodequeue.dequeue();
        //remember name of the node so it is not processed again
        QString nodeName(node->getName());
        //assign layer
        maxparent = getMaxParentLayer(node);
        node->setLayer(maxparent+1);
        if(node->getLayer()>maxlayer){
            maxlayer = node->getLayer();
        }
        //save node in hash, so it will be processed only once
        donehashtable[nodeName] = node;
        //Enqueue child nodes which are ready
        int num_children = 0;
        nnode_t** children = get_children_of(node->getOdinRef(), &num_children);

        // connect node to all children
        //enqueue children if not already done or in queue
        for(i=0; i< num_children; i++){
            nnode_t* nodeKid = children[i];
            QString kidName(nodeKid->name);
            LogicUnit* kidUnit = getReferenceToUnit(kidName);

            bool inQueue = nodequeue.contains(kidUnit);
            bool done = donehashtable.contains(kidName);

            if(!inQueue && !done && parentsDone(kidUnit,donehashtable)){
                nodequeue.enqueue(kidUnit);
            }
        }
    }
    maxlayer++;
    /*Locate all outputs at the very end of the graph*/
    QHash<QString, LogicUnit*>::const_iterator blockIterator = unithashtable.constBegin();
    for(int i = 0; i<=maxlayer;i++){
        blockIterator = unithashtable.constBegin();
        while(blockIterator != unithashtable.constEnd()){
            LogicUnit* actUnit = blockIterator.value();
            if(actUnit->getName().contains("top^out")){
                actUnit->setLayer(maxlayer);
            }

            ++blockIterator;
        }
    }

}

/*---------------------------------------------------------------------------------------------
 * (function: spreadLayers)
 *-------------------------------------------------------------------------------------------*/
void Container::spreadLayers()
{
    QHash<QString, LogicUnit*>::const_iterator blockIterator = unithashtable.constBegin();
    LogicUnit* lastUnit = NULL;
    int counter = 0;
    int offset = 200;
    int lastcount;
    for(int i = 0; i<=maxlayer;i++){
        blockIterator = unithashtable.constBegin();
        lastcount = 0;
        while(blockIterator != unithashtable.constEnd()){
            LogicUnit* actUnit = blockIterator.value();
            if(actUnit->getLayer()== i){
                actUnit->setPos(offset+15*counter,100.0+200*counter);
                actUnit->updateWires();
                lastUnit = actUnit;

                counter++;
            }
            lastcount++;
            if(maxcountPerLayer < counter){
                maxcountPerLayer = counter;
            }

            ++blockIterator;
        }
        offset = lastUnit->x()+200;
        counter = 0;

    }

}

/*---------------------------------------------------------------------------------------------
 * (function: startOdin)
 *-------------------------------------------------------------------------------------------*/
void Container::startOdin(){
    if(!odinStarted){
        myOdin->setFilename(myFilename);
        myOdin->startOdin();
        odinStarted = true;
        odinTable = myOdin->getNodeTable();
    }
}

/*---------------------------------------------------------------------------------------------
 * (function: createNodesFromOdin)
 *-------------------------------------------------------------------------------------------*/
int Container::createNodesFromOdin(){
    QHash<QString, nnode_t *>::const_iterator blockIterator = odinTable.constBegin();
    int items = 0;
     while(blockIterator != odinTable.constEnd()){
         QString actName = blockIterator.key();
         nnode_t* actOdinNode = blockIterator.value();
         operation_list odinType = actOdinNode->type;
         LogicUnit::UnitType type;

         switch(odinType){
         case CLOCK_NODE:
             type = LogicUnit::Clock;
             break;
         case INPUT_NODE:         
             type = LogicUnit::Input;
            break;
         case FF_NODE:
             type = LogicUnit::Latch;
             break;
         case OUTPUT_NODE:
             type = LogicUnit::Output;
             break;
         case LOGICAL_AND:
             type = LogicUnit::And;
             break;
         case LOGICAL_OR:
             type = LogicUnit::Or;
             break;
         case LOGICAL_NOT:
             type = LogicUnit::Not;
             break;
         case LOGICAL_NAND:
             type = LogicUnit::Nand;
             break;
         case LOGICAL_XOR:
             type = LogicUnit::Xor;
             break;
         case LOGICAL_XNOR:
             type = LogicUnit::Xnor;
             break;
         case LOGICAL_NOR:
             type = LogicUnit::Nor;
             break;
         case MUX_2:
             type = LogicUnit::MUX;
             break;
         case ADDER_FUNC:
             type = LogicUnit::ADDER_FUNC;
             break;
         case CARRY_FUNC:
             type = LogicUnit::CARRY_FUNC;
             break;
         default:
             type = LogicUnit::LogicGate;
             break;
         }
        //add Unit with name and type
         addUnit(actName, type,QPointF(100,100),actOdinNode);
         items++;
         ++blockIterator;
     }
     fprintf(stderr, "Created Nodes from Netlist: %d\n", items);
     return items;
}

/*---------------------------------------------------------------------------------------------
 * (function: parentsDone)
 *-------------------------------------------------------------------------------------------*/
bool Container::parentsDone(LogicUnit *unit, QHash<QString, LogicUnit *> donehashlist)
{
    bool result = true;
    QList<LogicUnit*> parents = unit->getParents();
    foreach(LogicUnit* parent,parents){
        if(!donehashlist.contains(parent->getName())){
            result = false;
        }
    }
    return result;
}

/*---------------------------------------------------------------------------------------------
 * (function: parentsDone)
 *-------------------------------------------------------------------------------------------*/
bool Container::parentsDone(LogicUnit *unit)
{
    bool result = true;
    QList<LogicUnit*> parents = unit->getParents();
    foreach(LogicUnit* parent,parents){
        if(!completeNodes.contains(parent->getName())){
            result = false;
        }
    }
    return result;
}

/*---------------------------------------------------------------------------------------------
 * (function: createConnectionsFromOdin)
 *-------------------------------------------------------------------------------------------*/
int Container::createConnectionsFromOdin()
{
    //only create connections if nodes are created
    if(myItemcount <= 0)
        return -1;
    //create all connections while traversing through the netlist
    //leave out the clock
    //(visual reasons. The clock should be added last to be at the right port)
    int i, j, conCount;
    conCount = 0;
    QQueue<LogicUnit*> nodequeue;
    QHash<QString, LogicUnit*> donehashtable;

    for (i = 0; i < verilog_netlist->num_top_input_nodes; i++){
        QString name = verilog_netlist->top_input_nodes[i]->name;
        nodequeue.enqueue(getReferenceToUnit(name));
    }

    // Enqueue constant nodes.
    nnode_t* constant_nodes[] = {verilog_netlist->gnd_node,
                                 verilog_netlist->vcc_node,
                                 verilog_netlist->pad_node};
    int num_constant_nodes = 3;
    for (i = 0; i < num_constant_nodes; i++){
        QString name = constant_nodes[i]->name;
        nodequeue.enqueue(getReferenceToUnit(name));
    }

    // go through the netlist. While doing so
    // remove nodes from the queue and add followup nodes
    LogicUnit* node;
    LogicUnit* clock;
    bool clockFound = false;
    while(!nodequeue.isEmpty()){
        node = nodequeue.dequeue();
        //remember name of the node so it is not processed again
        QString nodeName(node->getName());
        //save node in hash, so it will be processed only once
        donehashtable[nodeName] = node;
        //check each output pin for children.
        int output_number = node->getOdinRef()->num_output_pins;
        for(j=0; j<output_number;j++)
        {
            //Enqueue child nodes which are ready
            int num_children = 0;
            nnode_t** children = get_children_of_nodepin(node->getOdinRef(), &num_children,j);
            QHash<QString, nnode_t*> childrenDone;//used to not create a connection multiple times
            // connect node to all children
            //enqueue children if not already done or in queue
            for(i=0; i< num_children; i++){
                nnode_t* nodeKid = children[i];
                QString kidName(nodeKid->name);
                LogicUnit* kidUnit = getReferenceToUnit(kidName);

                bool inQueue = nodequeue.contains(getReferenceToUnit(kidName));
                bool done = donehashtable.contains(kidName);

                if(node->getOdinRef()->type != CLOCK_NODE)
                {
                    if(!childrenDone.contains(kidName)){
                        bool success = addConnectionHash(nodeName,kidName);
                        if(!success){
                            /*FIXME: This error is sometimes created by Odin as
                                        the method get_children_of(node->getOdinRef(), &num_children)
                                        sometimes returns a node itself as its child.*/
                            fprintf(stderr, "CONTAINER:  Warning: Node has itself in childlist in Odin II.\n");
                        }else{
                        //for hard blocks add the output pin of the created connection
                            Wire* newWire = getConnectionBetween(nodeName, kidName);
                            newWire->setMaxOutputPinNumber(
                                        getReferenceToUnit(nodeName)->getOdinRef()->num_output_pins);
                            newWire->setMyOutputPinNumber(j+1);
                            childrenDone[kidName] = nodeKid;
                            conCount++;
                        }
                    }
                }else{
                    clock = node;
                    clockFound = true;
                }

                if(!inQueue && !done && parentsDone(kidUnit,donehashtable)){
                    nodequeue.enqueue(kidUnit);
                }else if(!inQueue && !done && kidUnit->getOdinRef()->type == FF_NODE){
                    nodequeue.enqueue(kidUnit);
                }
            }
        }
    }

    //create clock connections
    int num_children = 0;
    if(clockFound){
        nnode_t** children = get_children_of(clock->getOdinRef(), &num_children);
        QString clockName = clock->getName();

        // connect clock to all children
        for(i=0; i< num_children; i++){
            nnode_t* nodeKid = children[i];
            QString kidName(nodeKid->name);
            addConnectionHash(clockName,kidName);
            conCount++;
        }
    }
    //return connection count
    return conCount;
}

/*---------------------------------------------------------------------------------------------
 * (function: readInFileOdinOnly)
 *-------------------------------------------------------------------------------------------*/
/**
  * Reads in the file in Odin and creates the nodes based on the netlist provided by odin.
  */
int Container::readInFileOdinOnly(){
    //used to measure the time, which is needed to create a graph
    clock_t start, finish;
    int cons;
    start = clock();
    //let odin ii parse in the file and return a hashtable of all nodes in the netlist
    startOdin();

    //iterate through the hashtable and create all nodes based on the type
    myItemcount = createNodesFromOdin();

     //create connections
    cons = createConnectionsFromOdin();
     if(myItemcount <= 0)
         return -1;

     finish = clock();
     fprintf(stderr, "Nodes: %d, Cons: %d, Time: %f\n", myItemcount, cons, (double)(finish - start)/CLOCKS_PER_SEC);
     return 0;

}

/*---------------------------------------------------------------------------------------------
 * (function: findByName)
 *-------------------------------------------------------------------------------------------*/
QHash<QString, LogicUnit *> Container::findByName(QString name)
{
    QHash<QString, LogicUnit *> result;
    QHash<QString, LogicUnit *> hash =unithashtable;
    QHash<QString, LogicUnit *>::const_iterator blockIterator = hash.constBegin();

     while(blockIterator != unithashtable.constEnd()){
         QString actName = blockIterator.key();
         if(actName.contains(name, Qt::CaseInsensitive)){
            result.insert(actName, blockIterator.value());
            blockIterator.value()->setBrush(QColor(23,255,255,255));
         }
         ++blockIterator;
     }
     return result;
}

/*---------------------------------------------------------------------------------------------
 * (function: getReferenceToUnit)
 *-------------------------------------------------------------------------------------------*/
LogicUnit * Container::getReferenceToUnit(QString name)
{
    if(!unithashtable.contains(name)){
        fprintf(stderr, "Container: Not all nodes could be connected!\n");
    }
    return unithashtable[name];
}

/*---------------------------------------------------------------------------------------------
 * (function: getMaxSimStep)
 *-------------------------------------------------------------------------------------------*/
int Container::getMaxSimStep()
{
    return 64;
}

/*---------------------------------------------------------------------------------------------
 * (function: getActSimiStep)
 *-------------------------------------------------------------------------------------------*/
int Container::getActSimStep()
{
    return actSimStep;
}

/*---------------------------------------------------------------------------------------------
 * (function: startSimulator)
 *-------------------------------------------------------------------------------------------*/
int Container::startSimulator()
{
    myOdin->setUpSimulation();
    myOdin->simulateNextWave();
    myOdin->endSimulation();
    copySimCyclesIntoNodes();
    return 0;
}

/*---------------------------------------------------------------------------------------------
 * (function: showSimulationStep)
 *-------------------------------------------------------------------------------------------*/
void Container::showSimulationStep(int cycle)
{
    actSimStep = cycle;
    //visit each node and update the output status of each one of them
    QHash<QString, nnode_t *>::const_iterator blockIterator = odinTable.constBegin();

    while(blockIterator != odinTable.constEnd()){
         QString name = blockIterator.key();
         LogicUnit* visNode = unithashtable[name];
         visNode->setCurrentCycle(cycle);
         ++blockIterator;
    }
    actSimStep = (simOffset+1)%64;
}

/*---------------------------------------------------------------------------------------------
 * (function: setEdge)
 *-------------------------------------------------------------------------------------------*/
void Container::setEdge(int i)
{
    if(odinStarted){
        myOdin->setEdge(i);
    }
}

/*---------------------------------------------------------------------------------------------
 * (function: getConnectionBetween)
 *-------------------------------------------------------------------------------------------*/
Wire *Container::getConnectionBetween(QString nodeName, QString kidName)
{
    Wire* result = NULL;
    QHash<QString, Wire *> wires;
    QHash<QString, LogicUnit *> hash =unithashtable;
    QHash<QString, LogicUnit *>::const_iterator blockIterator = hash.constBegin();

     while(blockIterator != unithashtable.constEnd()){
         QString actName = blockIterator.key();
         if(actName.compare(nodeName)==0){
             LogicUnit* actUnit = blockIterator.value();
             wires = actUnit->getOutConsHash();
         }
         ++blockIterator;
     }

     if(wires.contains(kidName)){
        result = wires.value(kidName);
     }
     return result;
}


