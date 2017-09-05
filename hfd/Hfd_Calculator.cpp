/**
 * Half Flux Diameter (HFD) calculation using the CImg library
 * and CCFits.
 *
 * Copyright (C) 2015 Carsten Schmitt
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Hfd_Calculator.h"
#include "SonyAlphaRemote_Helper.h"

#include <math.h>

#include <QFile>
#include <QImage>

namespace Hfd
{

QImage Calculator::readFile(const QString& inFilename)
{
    QFile imgFile(inFilename);
    imgFile.open(QIODevice::ReadOnly);

    QByteArray data = imgFile.readAll();
    return QImage::fromData(data, "JPG");

}

/**
 * Get all pixels inside a radius: http://stackoverflow.com/questions/14487322/get-all-pixel-array-inside-circle
 * Algorithm: http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
 */
bool Calculator::insideCircle(float inX /*pos of x*/, float inY /*pos of y*/, float inCenterX, float inCenterY, float inRadius) const
{
    return (pow(inX - inCenterX, 2.0) + pow(inY - inCenterY, 2.0) <= pow(inRadius, 2.0));
}

/**
 * Expects star centered in the middle of the image (in x and y) and mean background subtracted from image.
 *
 * HDF calculation: http://www005.upp.so-net.ne.jp/k_miyash/occ02/halffluxdiameter/halffluxdiameter_en.html
 *                  http://www.cyanogen.com/help/maximdl/Half-Flux.htm
 *
 * NOTE: Currently the accuracy is limited by the insideCircle function (-> sub-pixel accuracy).
 * NOTE: The HFD is estimated in case there is no flux (HFD ~ sqrt(2) * inOuterDiameter / 2).
 * NOTE: The outer diameter is usually a value which depends on the properties of the optical
 *       system and also on the seeing conditions. The HFD value calculated depends on this
 *       outer diameter value.
 */
float Calculator::calcHfd(const QImage& inImage, unsigned int inOuterDiameter)
{
    // Sum up all pixel values in whole circle
    float outerRadius = inOuterDiameter / 2;
    float sum = 0, sumDist = 0;
    int centerX = std::ceil(inImage.width() / 2.0);
    int centerY = std::ceil(inImage.height() / 2.0);

    for(int y=0; y<inImage.height(); y++)
    {
        for(int x=0; x<inImage.width(); x++)
        {
            if(insideCircle(x, y, centerX, centerY, outerRadius))
            {
                float gray = qGray(inImage.pixel(x, y));
                sum += gray;
                sumDist += gray * std::sqrt(std::pow((float)x - (float)centerX, 2.0f) + std::pow((float) y - (float)centerY, 2.0f));
            }
        }
    }


    // NOTE: Multiplying with 2 is required since actually just the HFR is calculated above
    return (sum ? 2.0 * sumDist / sum : std::sqrt(2.0) * outerRadius);
}

double Calculator::meanValue(const QImage &img) const
{
    double mean = 0;
    double pixelCount = 0;
    for(int y=0; y<img.height(); y++)
    {
        for(int x=0; x<img.width(); x++)
        {
            mean += qGray(img.pixel(x, y));
            pixelCount += 1;
        }
    }
    mean /= pixelCount;
    return mean;
}

QImage Calculator::scaledImage(const QImage &image, const double &mean) const
{
    QImage result(image.width(), image.height(), image.format());
    for(int y=0; y<image.height(); y++)
    {
        for(int x=0; x<image.width(); x++)
        {
            double gray = qGray(image.pixel(x, y));
            if(gray < mean)
                result.setPixel(x, y, qRgb(0, 0, 0));
            else
            {
                int value = int(gray - mean);
                result.setPixel(x, y, qRgb(value, value, value));
            }
        }
    }

    return result;
}


int Calculator::test(/* int argc, char *argv[]*/)
{
    QString files =
            ":/hfd/hfd/focus_test/jpeg/focus_test_596.jpg"
            ", :/hfd/hfd/focus_test/jpeg/focus_test_600.jpg"
            ", :/hfd/hfd/focus_test/jpeg/focus_test_601.jpg"
            ", :/hfd/hfd/focus_test/jpeg/focus_test_602.jpg"
            ", :/hfd/hfd/focus_test/jpeg/focus_test_607.jpg"
            ", :/hfd/hfd/focus_test/jpeg/focus_test_611.jpg"
            ", :/hfd/hfd/focus_test/jpeg/focus_test_617.jpg"
            ", :/hfd/hfd/focus_test/jpeg/focus_test_624.jpg"
            ", :/hfd/hfd/focus_test/jpeg/focus_test_630.jpg"
            ;

    QStringList fileList = files.split(", ", QString::SkipEmptyParts);

    foreach(QString fileName, fileList)
    {

        QImage image = readFile(fileName);

        double mean = meanValue(image);
        QImage colorScaledImage = scaledImage(image, mean);



        // Calc the HFD
        const unsigned int outerDiameter = 60;
        float hfd = calcHfd(colorScaledImage, outerDiameter /*outer diameter in px*/);

        SAR_INF(fileName << "--> hfd: " << hfd);
    }


    return 0;
}


} //namespace Hfd
