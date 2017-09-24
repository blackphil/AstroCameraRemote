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
#include <math.h>

#include <QFile>
#include <QImage>

namespace Hfd
{

class Calculator
{
public:

    QImage readFile(const QString& inFilename);

    /**
 * Get all pixels inside a radius: http://stackoverflow.com/questions/14487322/get-all-pixel-array-inside-circle
 * Algorithm: http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
 */
    bool insideCircle(float inX /*pos of x*/, float inY /*pos of y*/, float inCenterX, float inCenterY, float inRadius) const;

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
    double calcHfd(const QImage& inImage, unsigned int inOuterDiameter);

    double meanValue(const QImage& img) const;
    QImage scaledImage(const QImage& image, const double& mean) const;

    int test(/* int argc, char *argv[]*/);
};

} //namespace Hfd

