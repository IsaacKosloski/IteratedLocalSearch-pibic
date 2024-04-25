#include "scanner.h"


Scanner::Scanner() noexcept
{

}

Scanner::Scanner(string fileName) noexcept(false)
{
    this->fileName = fileName;
    dimensionOfNodes = -1;
    distanceType = -1;
    explicitDistanceFormat = -1;
    displayDataType = -1;
    edgeWeightFormat = -1;
    edgeWeightType = -1;
    col = -1;
    row = -1;
    components = nullptr;
    nodes = nullptr;
    nodesDistance = nullptr;

    readFile(fileName, components, nodes, nodesDistance);
}

void
Scanner::readFile(const string &fileName, Component* &components, Node* &nodes, Node* &nodesDistance)
{
    int ID = 1;
    int nextID = 1;
    double *positionComponents;
    string line = "";

    ifstream inputFile(fileName, ios::in);

    if (inputFile.is_open())
    {
        //getting the specifications of the selected "benchmark"
        while (getline(inputFile, line) && !specificationPart(line));

        components = new Component[dimensionOfNodes];
        nodes = new Node[dimensionOfNodes];
        nodesDistance = new Node[(dimensionOfNodes * dimensionOfNodes)];

        /*reading the rest of the file
         * getting the data part */
        /*if (distanceType == EXPLICIT)
        {
            if (explicitDistanceFormat == FUNCTION)
                ;
            else if (explicitDistanceFormat == FULL_MATRIX)
                ;
            else if (explicitDistanceFormat == UPPER_ROW)
                ;
            else if (explicitDistanceFormat == LOWER_ROW)
                ;
            else if (explicitDistanceFormat == UPPER_DIAG_ROW)
                ;
            else if (explicitDistanceFormat == LOWER_DIAG_ROW)
                ;
            else if (explicitDistanceFormat == UPPER_COL)
                ;
            else if (explicitDistanceFormat == LOWER_COL)
                ;
            else if (explicitDistanceFormat == UPPER_DIAG_COL)
                ;
            else if (explicitDistanceFormat == LOWER_DIAG_COL)
                ;
        }*/
        /*else*/if (distanceType == EUC_2D)
        {
            positionComponents = new double[2];
            while (getline(inputFile, line) && line != "EOF")
            {
                istringstream ssLine(line);
                ssLine >> ID >> positionComponents[0] >> positionComponents[1];
                components[ID - 1] = Component(2, positionComponents);
            }

            for (row = 0; row < dimensionOfNodes; row++)
            {
                for (col = 0; col < dimensionOfNodes; col++)
                {
                    nodesDistance[(row * dimensionOfNodes) + col]
                            = Node((col + 1) , true,
                                   sqrt((pow((components[row].positionComponents[0] - components[col].positionComponents[0]), 2)
                                         + (pow((components[row].positionComponents[1] - components[col].positionComponents[1]), 2)))));
                }
                if (nextID == dimensionOfNodes)
                    nextID = 0;

                nodes[row] = Node(row + 1, true, nodesDistance[(row * dimensionOfNodes) + nextID].distanceValue);
                nextID++;

            }
            for (row = 0; row < dimensionOfNodes; row++)
                delete[] components[row].positionComponents;

        }
        delete[] positionComponents;
        delete[] components;
        /*else if (distanceType == EUC_3D)
            distanceType = EUC_3D;
        else if (distanceType == MAX_2D)
            distanceType = MAX_2D;
        else if (distanceType == MAX_3D)
            distanceType = MAX_3D;
        else if (distanceType == MAN_2D)
            distanceType = MAN_2D;
        else if (distanceType == MAN_3D)
            distanceType = MAN_3D;
        else if (distanceType == CEIL_2D)
            distanceType = CEIL_2D;
        else if (distanceType == GEO)
            distanceType = GEO;
        else if (distanceType == ATT)
            distanceType = ATT;
        else if (distanceType == XRAY1)
            distanceType = XRAY1;
        else if (distanceType == XRAY2)
            distanceType = XRAY2;
        else if (distanceType == SPECIAL)
            distanceType = SPECIAL;*/


        inputFile.close();
    }
    else
        throw std::runtime_error("Unable to open file");

    inputFile.close();
}
bool
Scanner::specificationPart(string line)
{
    size_t found = 0;
    string stringData = "";
    //Verify the distance value type
    if (line.find("EDGE_WEIGHT_TYPE") != string::npos)
    {
        found = line.find(": ");
        if (found != string::npos)
        {/**/
            stringData = line.substr(found+2);
            if (stringData == "EXPLICIT")
                distanceType = EXPLICIT;
            else if (stringData == "EUC_2D")
                distanceType = EUC_2D;
            else if (stringData == "EUC_3D")
                distanceType = EUC_3D;
            else if (stringData == "MAX_2D")
                distanceType = MAX_2D;
            else if (stringData == "MAX_3D")
                distanceType = MAX_3D;
            else if (stringData == "MAN_2D")
                distanceType = MAN_2D;
            else if (stringData == "MAN_3D")
                distanceType = MAN_3D;
            else if (stringData == "CEIL_2D")
                distanceType = CEIL_2D;
            else if (stringData == "GEO")
                distanceType = GEO;
            else if (stringData == "ATT")
                distanceType = ATT;
            else if (stringData == "XRAY1")
                distanceType = XRAY1;
            else if (stringData == "XRAY2")
                distanceType = XRAY2;
            else if (stringData == "SPECIAL")
                distanceType = SPECIAL;
            else
                throw std::invalid_argument("Not acceptable DISTANCE TYPE.");
        }
    }
        //If the distance type is equal to explicit type, then there ir the field of edge distance format
    else if (distanceType == EXPLICIT && line.find("EDGE_WEIGHT_FORMAT") != string::npos)
    {
        found = line.find(": ");
        if (found != string::npos)
        {
            stringData = line.substr(found + 2);
            if (stringData == "FUNCTION")
                explicitDistanceFormat = FUNCTION;
            else if (stringData == "FULL_MATRIX")
                explicitDistanceFormat = FULL_MATRIX;
            else if (stringData == "UPPER_ROW")
                explicitDistanceFormat = UPPER_ROW;
            else if (stringData == "LOWER_ROW")
                explicitDistanceFormat = LOWER_ROW;
            else if (stringData == "UPPER_DIAG_ROW")
                explicitDistanceFormat = UPPER_DIAG_ROW;
            else if (stringData == "LOWER_DIAG_ROW")
                explicitDistanceFormat = LOWER_DIAG_ROW;
            else if (stringData == "UPPER_COL")
                explicitDistanceFormat = UPPER_COL;
            else if (stringData == "LOWER_COL")
                explicitDistanceFormat = LOWER_COL;
            else if (stringData == "UPPER_DIAG_COL")
                explicitDistanceFormat = UPPER_DIAG_COL;
            else if (stringData == "LOWER_DIAG_COL")
                explicitDistanceFormat = LOWER_DIAG_COL;
            else
                throw std::invalid_argument("Not acceptable DISTANCE FORMAT.");
        }
    }
    else if (line.find("DISPLAY_DATA_TYPE") != string::npos)
    {
        found = line.find(": ");
        if (found != string::npos)
        {
            stringData = line.substr(found + 2);
            if (stringData == "TWOD_DISPLAY")
                displayDataType = TWOD_DISPLAY;
            else if (stringData == "NO_DISPLAY")
                displayDataType = NO_DISPLAY;
            else if (stringData == "COORD_DISPLAY")
                displayDataType = COORD_DISPLAY;
            else
                throw std::invalid_argument("Not acceptable DISPLAY DATA TYPE");

        }
    }
        //Verifying the file type
    else if (line.find("TYPE") != string::npos)
    {
        found = line.find(": ");
        if (found != string::npos)
        {
            stringData = line.substr(found+2);
            if (stringData != "TSP")
                throw std::invalid_argument("Not acceptable file TYPE.");
        }
    }
    else if (line.find("DIMENSION") != string::npos)
    {
        found = line.find(": ");
        if (found != string::npos)
        {
            stringData = line.substr(found+2);
            dimensionOfNodes = stoi(stringData);
        }
    }
    else if (line.find("NODE_COORD_SECTION") != string::npos)
        return true;

    return false;
}
