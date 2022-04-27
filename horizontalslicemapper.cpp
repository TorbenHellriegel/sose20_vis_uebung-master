#include "iostream"
#include "horizontalslicemapper.h"

HorizontalSliceMapper::HorizontalSliceMapper()
{

}

HorizontalSliceMapper::~HorizontalSliceMapper()
{

}

void HorizontalSliceMapper::setDataSource(FlowDataSource *flowDataSource)
{
    dataSource = flowDataSource;
}

QImage HorizontalSliceMapper::mapSliceToImage(int iz)
{
    // Crate an image the size of a horizontal slice of the tornado.
    QImage img(16, 16, QImage::Format_RGB32);

    float c;
    QRgb color;

    // For each coordinate in the Slice map the x value of said
    // coordinate to a corresponding pixel in the image.
    // If the value is below 0 the pixel will be blue
    // and if the value is above 0 it will be red.
    for(int iy = 0; iy < dataSource->Y; iy++)
    {
        for(int ix = 0; ix < dataSource->X; ix++)
        {
            c = dataSource->getDataValue(iz, iy, ix, 0) * 3*255;
            if(c < 0)
            {
                color = qRgb(0, 0, -c);
            }
            else
            {
                color = qRgb(c, 0, 0);
            }
            img.setPixelColor(ix, iy, color);
        }
    }
    return img;
}
