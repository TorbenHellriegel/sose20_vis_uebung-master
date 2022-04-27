#include "horizontalcontourlinesmapper.h"
#include <iostream>

HorizontalContourLinesMapper::HorizontalContourLinesMapper()
{

}

HorizontalContourLinesMapper::~HorizontalContourLinesMapper()
{
    vector.clear();
}

void HorizontalContourLinesMapper::setDataSource(FlowDataSource *flowDataSource)
{
    dataSource = flowDataSource;
}

QVector<QVector3D> HorizontalContourLinesMapper::mapSliceToContourLineSegments(int iz, float iso)
{
    // Clear the vector in case there is already data saved.
    vector.clear();
    isoline = iso;
    int x = dataSource->X;
    int y = dataSource->X;
    //bool plus[y*x];
    int plus[x*y];
    bool v0, v1, v2, v3;

    // Calculate the binary encodement for each square betwenn 4 vertex points.
    for(int iy = 0; iy < y-1; iy++)
    {
        for(int ix = 0; ix < x-1; ix++)
        {

            v0 = isoline < dataSource->getDataValue(iz, iy, ix, 0);
            v1 = isoline < dataSource->getDataValue(iz, iy, ix+1, 0);
            v2 = isoline < dataSource->getDataValue(iz, iy+1, ix+1, 0);
            v3 = isoline < dataSource->getDataValue(iz, iy+1, ix, 0);

            plus[iy*x + ix] = 1000*v3 + 100*v2 + 10*v1 + v0;
        }
    }

    // Check for each square betwenn 4 vertex
    // points if and where a line has to be drawn.
    for(int iy = 0; iy < y-1; iy++)
    {
        for(int ix = 0; ix < x-1; ix++)
        {
            // Line bottom left.
            if(plus[iy*x+ix] == 1 ||plus[iy*x+ix] == 1110)
            {
                vector.append(isoCrossingBetweenTwoVertices(ix, iy, ix+1, iy, iz));
                vector.append(isoCrossingBetweenTwoVertices(ix, iy, ix, iy+1, iz));
            }
            // Line bottom right.
            else if(plus[iy*x+ix] == 10 ||plus[iy*x+ix] == 1101)
            {
                vector.append(isoCrossingBetweenTwoVertices(ix, iy, ix+1, iy, iz));
                vector.append(isoCrossingBetweenTwoVertices(ix+1, iy, ix+1, iy+1, iz));
            }
            // Line top left.
            else if(plus[iy*x+ix] == 111 ||plus[iy*x+ix] == 1000)
            {
                vector.append(isoCrossingBetweenTwoVertices(ix, iy, ix, iy+1, iz));
                vector.append(isoCrossingBetweenTwoVertices(ix, iy+1, ix+1, iy+1, iz));
            }
            // Line top right.
            else if(plus[iy*x+ix] == 100 ||plus[iy*x+ix] == 1011)
            {
                vector.append(isoCrossingBetweenTwoVertices(ix+1, iy, ix+1, iy+1, iz));
                vector.append(isoCrossingBetweenTwoVertices(ix, iy+1, ix+1, iy+1, iz));
            }
            // Line horizontal.
            else if(plus[iy*x+ix] == 11 ||plus[iy*x+ix] == 1100)
            {
                vector.append(isoCrossingBetweenTwoVertices(ix, iy, ix, iy+1, iz));
                vector.append(isoCrossingBetweenTwoVertices(ix+1, iy, ix+1, iy+1, iz));
            }
            // Line vertical.
            else if(plus[iy*x+ix] == 110 ||plus[iy*x+ix] == 1001)
            {
                vector.append(isoCrossingBetweenTwoVertices(ix, iy, ix+1, iy, iz));
                vector.append(isoCrossingBetweenTwoVertices(ix, iy+1, ix+1, iy+1, iz));
            }
            // Line ambiguous 1.
            else if(plus[iy*x+ix] == 101)
            {
                // Calculate the middle point to determine wich lines to draw.
                if(((dataSource->getDataValue(iz,iy,ix,0)
                   +dataSource->getDataValue(iz,iy,ix+1,0)
                   +dataSource->getDataValue(iz,iy+1,ix+1,0)
                   +dataSource->getDataValue(iz,iy+1,ix,0))/4) < isoline)
                {
                    // Line top right.
                    vector.append(isoCrossingBetweenTwoVertices(ix+1, iy, ix+1, iy+1, iz));
                    vector.append(isoCrossingBetweenTwoVertices(ix, iy+1, ix+1, iy+1, iz));
                    // Line bottom left.
                    vector.append(isoCrossingBetweenTwoVertices(ix, iy, ix+1, iy, iz));
                    vector.append(isoCrossingBetweenTwoVertices(ix, iy, ix, iy+1, iz));
                }
                else
                {
                    // Line top left.
                    vector.append(isoCrossingBetweenTwoVertices(ix, iy, ix, iy+1, iz));
                    vector.append(isoCrossingBetweenTwoVertices(ix, iy+1, ix+1, iy+1, iz));
                    // Line bottom right.
                    vector.append(isoCrossingBetweenTwoVertices(ix, iy, ix+1, iy, iz));
                    vector.append(isoCrossingBetweenTwoVertices(ix+1, iy, ix+1, iy+1, iz));
                }
            }
            // Line ambiguous 2.
            else if(plus[iy*x+ix] == 1010)
            {
                // Calculate the middle point to determine wich lines to draw.
                if(((dataSource->getDataValue(iz,iy,ix,0)
                   +dataSource->getDataValue(iz,iy,ix+1,0)
                   +dataSource->getDataValue(iz,iy+1,ix+1,0)
                   +dataSource->getDataValue(iz,iy+1,ix,0))/4) < isoline)
                {
                    // Line top left.
                    vector.append(isoCrossingBetweenTwoVertices(ix, iy, ix, iy+1, iz));
                    vector.append(isoCrossingBetweenTwoVertices(ix, iy+1, ix+1, iy+1, iz));
                    // Line bottom right.
                    vector.append(isoCrossingBetweenTwoVertices(ix, iy, ix+1, iy, iz));
                    vector.append(isoCrossingBetweenTwoVertices(ix+1, iy, ix+1, iy+1, iz));
                }
                else
                {
                    // Line top right.
                    vector.append(isoCrossingBetweenTwoVertices(ix+1, iy, ix+1, iy+1, iz));
                    vector.append(isoCrossingBetweenTwoVertices(ix, iy+1, ix+1, iy+1, iz));
                    // Line bottom left.
                    vector.append(isoCrossingBetweenTwoVertices(ix, iy, ix+1, iy, iz));
                    vector.append(isoCrossingBetweenTwoVertices(ix, iy, ix, iy+1, iz));
                }
            }
            // No line.
            else
            {
                //float delta = (val[0] * val[3] - val[1] * val[2]) / (val[0] - val[1] - val[2] + val[3]);
            }
        }
    }
    return vector;
}

QVector3D HorizontalContourLinesMapper::isoCrossingBetweenTwoVertices(int x1, int y1, int x2, int y2, float z)
{
    QVector3D cVec;
    float v1, v2, alpha, newX, newY;

    // Determine the value at the given vertex points.
    v1 = dataSource->getDataValue(z, y1, x1, 0);
    v2 = dataSource->getDataValue(z, y2, x2, 0);
    alpha = (isoline - v1)/(v2 - v1);

    // Calculate where between the two vertex points the isoline will be.
    newX = (1 - alpha)*x1 + alpha*x2;
    newY = (1 - alpha)*y1 + alpha*y2;

    // Set the coordinates for the isoline.
    cVec.setX(newX);
    cVec.setY(newY);
    cVec.setZ(z);

    return cVec/15;
}
