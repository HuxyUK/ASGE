//  Copyright (c) 2021 James Huxtable. All rights reserved.
//
//  This work is licensed under the terms of the MIT license.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.

//! @file Colours.hpp
//! @brief Struct @ref ASGE::Colour, Namespace @ref ASGE::COLOURS

#include <array>
#pragma once
namespace ASGE {
  /**
   * @brief RGB Helper.
   *
   * Used to represent the RGB values of a colour. The range of
   * values here are from 0 to 1, with 1 directly mapping to 255
   * and 0 mapping to 0.
   */
  struct Colour
	{
		float r = 1; /**< The red component.   */
		float g = 1; /**< The green component. */
		float b = 1; /**< The blue component.  */

		/**
		* Default constructor.
		* The constructor takes an array of 3 floats. These
		* are then mapped on the RGB values inside the struct.
		*/
    explicit constexpr Colour(const std::array<float, 3>& rgb) noexcept :
      r(rgb[0]), g(rgb[1]), b(rgb[2])
    {
    }

    constexpr Colour(const float red, const float green, const float blue) noexcept :
      r(red), g(green), b(blue)
    {
    }
  };

    /**
    * @namespace ASGE::COLOURS
    * @brief predefined RGB values
    */
	namespace COLOURS {
		constexpr const Colour ALICEBLUE {0.941F, 0.973F, 1.000F};             /**< ALICEBLUE[] = { 0.941F, 0.973F, 1.000F }         */
		constexpr const Colour ANTIQUEWHITE{ 0.980F, 0.922F, 0.843F };         /**< ANTIQUEWHITE[] = { 0.980F, 0.922F, 0.843F }      */
		constexpr const Colour AQUA { 0.000F, 1.000F, 1.000F };                 /**< AQUA { 0.000F, 1.000F, 1.000F }                 */
		constexpr const Colour AQUAMARINE { 0.498F, 1.000F, 0.831F };           /**< AQUAMARINE { 0.498F, 1.000F, 0.831F }           */
		constexpr const Colour AZURE { 0.941F, 1.000F, 1.000F };                /**< AZURE { 0.941F, 1.000F, 1.000F }                */
		constexpr const Colour BEIGE { 0.961F, 0.961F, 0.863F };                /**< BEIGE { 0.961F, 0.961F, 0.863F }                */
		constexpr const Colour BISQUE { 1.000F, 0.894F, 0.769F };               /**< BISQUE { 1.000F, 0.894F, 0.769F }               */
		constexpr const Colour BLACK { 0.000F, 0.000F, 0.000F };                /**< BLACK { 0.000F, 0.000F, 0.000F }                */
		constexpr const Colour BLANCHEDALMOND { 1.000F, 0.922F, 0.804F };       /**< BLANCHEDALMOND { 1.000F, 0.922F, 0.804F }       */
		constexpr const Colour BLUE { 0.000F, 0.000F, 1.000F };                 /**< BLUE { 0.000F, 0.000F, 1.000F }                 */
		constexpr const Colour BLUEVIOLET { 0.541F, 0.169F, 0.886F };           /**< BLUEVIOLET { 0.541F, 0.169F, 0.886F }           */
		constexpr const Colour BROWN { 0.647F, 0.165F, 0.165F };                /**< BROWN { 0.647F, 0.165F, 0.165F }                */
		constexpr const Colour BURLYWOOD { 0.871F, 0.722F, 0.529F };            /**< BURLYWOOD { 0.871F, 0.722F, 0.529F }            */
		constexpr const Colour CADETBLUE { 0.373F, 0.620F, 0.627F };            /**< CADETBLUE { 0.373F, 0.620F, 0.627F }            */
		constexpr const Colour CHARTREUSE { 0.498F, 1.000F, 0.000F };           /**< CHARTREUSE { 0.498F, 1.000F, 0.000F }           */
		constexpr const Colour CHOCOLATE { 0.824F, 0.412F, 0.118F };            /**< CHOCOLATE { 0.824F, 0.412F, 0.118F }            */
		constexpr const Colour CORAL { 1.000F, 0.498F, 0.314F };                /**< CORAL { 1.000F, 0.498F, 0.314F }                */
		constexpr const Colour CORNFLOWERBLUE { 0.392F, 0.584F, 0.929F };       /**< CORNFLOWERBLUE { 0.392F, 0.584F, 0.929F }       */
		constexpr const Colour CORNSILK { 1.000F, 0.973F, 0.863F };             /**< CORNSILK { 1.000F, 0.973F, 0.863F }             */
		constexpr const Colour CRIMSON { 0.863F, 0.078F, 0.235F };              /**< CRIMSON { 0.863F, 0.078F, 0.235F }              */
		constexpr const Colour CYAN { 0.000F, 1.000F, 1.000F };                 /**< CYAN { 0.000F, 1.000F, 1.000F }                 */
		constexpr const Colour DARKBLUE { 0.000F, 0.000F, 0.545F };             /**< DARKBLUE { 0.000F, 0.000F, 0.545F }             */
		constexpr const Colour DARKCYAN { 0.000F, 0.545F, 0.545F };             /**< DARKCYAN { 0.000F, 0.545F, 0.545F }             */
		constexpr const Colour DARKGOLDENROD { 0.722F, 0.525F, 0.043F };        /**< DARKGOLDENROD { 0.722F, 0.525F, 0.043F }        */
		constexpr const Colour DARKGRAY { 0.663F, 0.663F, 0.663F };             /**< DARKGRAY { 0.663F, 0.663F, 0.663F }             */
		constexpr const Colour DARKGREEN { 0.000F, 0.392F, 0.000F };            /**< DARKGREEN { 0.000F, 0.392F, 0.000F }            */
		constexpr const Colour DARKGREY { 0.663F, 0.663F, 0.663F };             /**< DARKGREY { 0.663F, 0.663F, 0.663F }             */
		constexpr const Colour DARKKHAKI { 0.741F, 0.718F, 0.420F };            /**< DARKKHAKI { 0.741F, 0.718F, 0.420F }            */
		constexpr const Colour DARKMAGENTA { 0.545F, 0.000F, 0.545F };          /**< DARKMAGENTA { 0.545F, 0.000F, 0.545F }          */
		constexpr const Colour DARKOLIVEGREEN { 0.333F, 0.420F, 0.184F };       /**< DARKOLIVEGREEN { 0.333F, 0.420F, 0.184F }       */
		constexpr const Colour DARKORANGE { 1.000F, 0.549F, 0.000F };           /**< DARKORANGE { 1.000F, 0.549F, 0.000F }           */
		constexpr const Colour DARKORCHID { 0.600F, 0.196F, 0.800F };           /**< DARKORCHID { 0.600F, 0.196F, 0.800F }           */
		constexpr const Colour DARKRED { 0.545F, 0.000F, 0.000F };              /**< DARKRED { 0.545F, 0.000F, 0.000F }              */
		constexpr const Colour DARKSALMON { 0.914F, 0.588F, 0.478F };           /**< DARKSALMON { 0.914F, 0.588F, 0.478F }           */
		constexpr const Colour DARKSEAGREEN { 0.561F, 0.737F, 0.561F };         /**< DARKSEAGREEN { 0.561F, 0.737F, 0.561F }         */
		constexpr const Colour DARKSLATEBLUE { 0.282F, 0.239F, 0.545F };        /**< DARKSLATEBLUE { 0.282F, 0.239F, 0.545F }        */
		constexpr const Colour DARKSLATEGRAY { 0.184F, 0.310F, 0.310F };        /**< DARKSLATEGRAY { 0.184F, 0.310F, 0.310F }        */
		constexpr const Colour DARKTURQUOISE { 0.000F, 0.808F, 0.820F };        /**< DARKTURQUOISE { 0.000F, 0.808F, 0.820F }        */
		constexpr const Colour DARKVIOLET { 0.580F, 0.000F, 0.827F };           /**< DARKVIOLET { 0.580F, 0.000F, 0.827F }           */
		constexpr const Colour DEEPPINK { 1.000F, 0.078F, 0.576F };             /**< DEEPPINK { 1.000F, 0.078F, 0.576F }             */
		constexpr const Colour DEEPSKYBLUE { 0.000F, 0.749F, 1.000F };          /**< DEEPSKYBLUE { 0.000F, 0.749F, 1.000F }          */
		constexpr const Colour DIMGRAY { 0.412F, 0.412F, 0.412F };              /**< DIMGRAY { 0.412F, 0.412F, 0.412F }              */
		constexpr const Colour DIMGREY { 0.412F, 0.412F, 0.412F };              /**< DIMGREY { 0.412F, 0.412F, 0.412F }              */
		constexpr const Colour DODGERBLUE { 0.118F, 0.565F, 1.000F };           /**< DODGERBLUE { 0.118F, 0.565F, 1.000F }           */
		constexpr const Colour FIREBRICK { 0.698F, 0.133F, 0.133F };            /**< FIREBRICK { 0.698F, 0.133F, 0.133F }            */
		constexpr const Colour FLORALWHITE { 1.000F, 0.980F, 0.941F };          /**< FLORALWHITE { 1.000F, 0.980F, 0.941F }          */
		constexpr const Colour FORESTGREEN { 0.133F, 0.545F, 0.133F };          /**< FORESTGREEN { 0.133F, 0.545F, 0.133F }          */
		constexpr const Colour FUCHSIA { 1.000F, 0.000F, 1.000F };              /**< FUCHSIA { 1.000F, 0.000F, 1.000F }              */
		constexpr const Colour GAINSBORO { 0.863F, 0.863F, 0.863F };            /**< GAINSBORO { 0.863F, 0.863F, 0.863F }            */
		constexpr const Colour GHOSTWHITE { 0.973F, 0.973F, 1.000F };           /**< GHOSTWHITE { 0.973F, 0.973F, 1.000F }           */
		constexpr const Colour GOLD { 1.000F, 0.843F, 0.000F };                 /**< GOLD { 1.000F, 0.843F, 0.000F }                 */
		constexpr const Colour GOLDENROD { 0.855F, 0.647F, 0.125F };            /**< GOLDENROD { 0.855F, 0.647F, 0.125F }            */
		constexpr const Colour GRAY { 0.502F, 0.502F, 0.502F };                 /**< GRAY { 0.502F, 0.502F, 0.502F }                 */
    constexpr const Colour GRAYBLACK { 0.100F, 0.100F, 0.100F };            /**< GRAYBLACK { 0.100F, 0.100F, 0.100F }            */
		constexpr const Colour GREEN { 0.000F, 0.502F, 0.000F };                /**< GREEN { 0.000F, 0.502F, 0.000F }                */
		constexpr const Colour GREENYELLOW { 0.678F, 1.000F, 0.184F };          /**< GREENYELLOW { 0.678F, 1.000F, 0.184F }          */
		constexpr const Colour GREY { 0.502F, 0.502F, 0.502F };                 /**< GREY { 0.502F, 0.502F, 0.502F }                 */
    constexpr const Colour GREYBLACK { 0.100F, 0.100F, 0.100F };            /**< GREYBLACK { 0.100F, 0.100F, 0.100F }            */
		constexpr const Colour HONEYDEW { 0.941F, 1.000F, 0.941F };             /**< HONEYDEW { 0.941F, 1.000F, 0.941F }             */
		constexpr const Colour HOTPINK { 1.000F, 0.412F, 0.706F };              /**< HOTPINK { 1.000F, 0.412F, 0.706F }              */
		constexpr const Colour INDIANRED { 0.804F, 0.361F, 0.361F };            /**< INDIANRED { 0.804F, 0.361F, 0.361F }            */
		constexpr const Colour INDIGO { 0.294F, 0.000F, 0.510F };               /**< INDIGO { 0.294F, 0.000F, 0.510F }               */
		constexpr const Colour IVORY { 1.000F, 1.000F, 0.941F };                /**< IVORY { 1.000F, 1.000F, 0.941F }                */
		constexpr const Colour KHAKI { 0.941F, 0.902F, 0.549F };                /**< KHAKI { 0.941F, 0.902F, 0.549F }                */
		constexpr const Colour LAVENDER { 0.902F, 0.902F, 0.980F };             /**< LAVENDER { 0.902F, 0.902F, 0.980F }             */
		constexpr const Colour LAVENDERBLUSH { 1.000F, 0.941F, 0.961F };        /**< LAVENDERBLUSH { 1.000F, 0.941F, 0.961F }        */
		constexpr const Colour LAWNGREEN { 0.486F, 0.988F, 0.000F };            /**< LAWNGREEN { 0.486F, 0.988F, 0.000F }            */
		constexpr const Colour LEMONCHIFFON { 1.000F, 0.980F, 0.804F };         /**< LEMONCHIFFON { 1.000F, 0.980F, 0.804F }         */
		constexpr const Colour LIGHTBLUE { 0.678F, 0.847F, 0.902F };            /**< LIGHTBLUE { 0.678F, 0.847F, 0.902F }            */
		constexpr const Colour LIGHTCORAL { 0.941F, 0.502F, 0.502F };           /**< LIGHTCORAL { 0.941F, 0.502F, 0.502F }           */
		constexpr const Colour LIGHTCYAN { 0.878F, 1.000F, 1.000F };            /**< LIGHTCYAN { 0.878F, 1.000F, 1.000F }            */
		constexpr const Colour LIGHTGOLDENRODYELLOW { 0.980F, 0.980F, 0.824F }; /**< LIGHTGOLDENRODYELLOW { 0.980F, 0.980F, 0.824F } */
		constexpr const Colour LIGHTGRAY { 0.827F, 0.827F, 0.827F };            /**< LIGHTGRAY { 0.827F, 0.827F, 0.827F }            */
		constexpr const Colour LIGHTGREEN { 0.565F, 0.933F, 0.565F };           /**< LIGHTGREEN { 0.565F, 0.933F, 0.565F }           */
		constexpr const Colour LIGHTGREY { 0.827F, 0.827F, 0.827F };            /**< LIGHTGREY { 0.827F, 0.827F, 0.827F }            */
		constexpr const Colour LIGHTPINK { 1.000F, 0.714F, 0.757F };            /**< LIGHTPINK { 1.000F, 0.714F, 0.757F }            */
		constexpr const Colour LIGHTSALMON { 1.000F, 0.627F, 0.478F };          /**< LIGHTSALMON { 1.000F, 0.627F, 0.478F }          */
		constexpr const Colour LIGHTSEAGREEN { 0.125F, 0.698F, 0.667F };        /**< LIGHTSEAGREEN { 0.125F, 0.698F, 0.667F }        */
		constexpr const Colour LIGHTSKYBLUE { 0.529F, 0.808F, 0.980F };         /**< LIGHTSKYBLUE { 0.529F, 0.808F, 0.980F }         */
		constexpr const Colour LIGHTSLATEGRAY { 0.467F, 0.533F, 0.600F };       /**< LIGHTSLATEGRAY { 0.467F, 0.533F, 0.600F }       */
		constexpr const Colour LIGHTSLATEGREY { 0.467F, 0.533F, 0.600F };       /**< LIGHTSLATEGREY { 0.467F, 0.533F, 0.600F }       */
		constexpr const Colour LIGHTSTEELBLUE { 0.690F, 0.769F, 0.871F };       /**< LIGHTSTEELBLUE { 0.690F, 0.769F, 0.871F }       */
		constexpr const Colour LIGHTYELLOW { 1.000F, 1.000F, 0.878F };          /**< LIGHTYELLOW { 1.000F, 1.000F, 0.878F }          */
		constexpr const Colour LIME { 0.000F, 1.000F, 0.000F };                 /**< LIME { 0.000F, 1.000F, 0.000F };                */
		constexpr const Colour LIMEGREEN { 0.196F, 0.804F, 0.196F };            /**< LIMEGREEN { 0.196F, 0.804F, 0.196F }            */
		constexpr const Colour LINEN { 0.980F, 0.941F, 0.902F };                /**< LINEN { 0.980F, 0.941F, 0.902F }                */
		constexpr const Colour MAGENTA { 1.000F, 0.000F, 1.000F };              /**< MAGENTA { 1.000F, 0.000F, 1.000F }              */
		constexpr const Colour MAROON { 0.502F, 0.000F, 0.000F };               /**< MAROON { 0.502F, 0.000F, 0.000F }               */
		constexpr const Colour MEDIUMAQUAMARINE { 0.400F, 0.804F, 0.667F };     /**< MEDIUMAQUAMARINE { 0.400F, 0.804F, 0.667F }     */
		constexpr const Colour MEDIUMBLUE { 0.000F, 0.000F, 0.804F };           /**< MEDIUMBLUE { 0.000F, 0.000F, 0.804F }           */
		constexpr const Colour MEDIUMORCHID { 0.729F, 0.333F, 0.827F };         /**< MEDIUMORCHID { 0.729F, 0.333F, 0.827F }         */
		constexpr const Colour MEDIUMPURPLE { 0.576F, 0.439F, 0.859F };         /**< MEDIUMPURPLE { 0.576F, 0.439F, 0.859F }         */
		constexpr const Colour MEDIUMSEAGREEN { 0.235F, 0.702F, 0.443F };       /**< MEDIUMSEAGREEN { 0.235F, 0.702F, 0.443F }       */
		constexpr const Colour MEDIUMSLATEBLUE { 0.482F, 0.408F, 0.933F };      /**< MEDIUMSLATEBLUE { 0.482F, 0.408F, 0.933F }      */
		constexpr const Colour MEDIUMSPRINGGREEN { 0.000F, 0.980F, 0.604F };    /**< MEDIUMSPRINGGREEN { 0.000F, 0.980F, 0.604F }    */
		constexpr const Colour MEDIUMTURQUOISE { 0.282F, 0.820F, 0.800F };      /**< MEDIUMTURQUOISE { 0.282F, 0.820F, 0.800F }      */
		constexpr const Colour MEDIUMVIOLETRED { 0.780F, 0.082F, 0.522F };      /**< MEDIUMVIOLETRED { 0.780F, 0.082F, 0.522F }      */
		constexpr const Colour MIDNIGHTBLUE { 0.098F, 0.098F, 0.439F };         /**< MIDNIGHTBLUE { 0.098F, 0.098F, 0.439F }         */
		constexpr const Colour MINTCREAM { 0.961F, 1.000F, 0.980F };            /**< MINTCREAM { 0.961F, 1.000F, 0.980F }            */
		constexpr const Colour MISTYROSE { 1.000F, 0.894F, 0.882F };            /**< MISTYROSE { 1.000F, 0.894F, 0.882F }            */
		constexpr const Colour MOCCASIN { 1.000F, 0.894F, 0.710F };             /**< MOCCASIN { 1.000F, 0.894F, 0.710F }             */
		constexpr const Colour NAVAJOWHITE { 1.000F, 0.871F, 0.678F };          /**< NAVAJOWHITE { 1.000F, 0.871F, 0.678F }          */
		constexpr const Colour NAVY { 0.000F, 0.000F, 0.502F };                 /**< NAVY { 0.000F, 0.000F, 0.502F }                 */
		constexpr const Colour OLDLACE { 0.992F, 0.961F, 0.902F };              /**< OLDLACE { 0.992F, 0.961F, 0.902F }              */
		constexpr const Colour OLIVE { 0.502F, 0.502F, 0.000F };                /**< OLIVE { 0.502F, 0.502F, 0.000F }                */
		constexpr const Colour OLIVEDRAB { 0.420F, 0.557F, 0.137F };            /**< OLIVEDRAB { 0.420F, 0.557F, 0.137F }            */
		constexpr const Colour ORANGE { 1.000F, 0.647F, 0.000F };               /**< ORANGE { 1.000F, 0.647F, 0.000F }               */
		constexpr const Colour ORANGERED { 1.000F, 0.271F, 0.000F };            /**< ORANGERED { 1.000F, 0.271F, 0.000F }            */
		constexpr const Colour ORCHID { 0.855F, 0.439F, 0.839F };               /**< ORCHID { 0.855F, 0.439F, 0.839F }               */
		constexpr const Colour PALEGOLDENROD { 0.933F, 0.910F, 0.667F };        /**< PALEGOLDENROD { 0.933F, 0.910F, 0.667F }        */
		constexpr const Colour PALEGREEN { 0.596F, 0.984F, 0.596F };            /**< PALEGREEN { 0.596F, 0.984F, 0.596F }            */
		constexpr const Colour PALETURQUOISE { 0.686F, 0.933F, 0.933F };        /**< PALETURQUOISE { 0.686F, 0.933F, 0.933F }        */
		constexpr const Colour PALEVIOLETRED { 0.859F, 0.439F, 0.576F };        /**< PALEVIOLETRED { 0.859F, 0.439F, 0.576F }        */
		constexpr const Colour PAPAYAWHIP { 1.000F, 0.937F, 0.835F };           /**< PAPAYAWHIP { 1.000F, 0.937F, 0.835F }           */
		constexpr const Colour PEACHPUFF { 1.000F, 0.855F, 0.725F };            /**< PEACHPUFF { 1.000F, 0.855F, 0.725F }            */
		constexpr const Colour PERU { 0.804F, 0.522F, 0.247F };                 /**< PERU { 0.804F, 0.522F, 0.247F }                 */
		constexpr const Colour PINK { 1.000F, 0.753F, 0.796F };                 /**< PINK { 1.000F, 0.753F, 0.796F }                 */
		constexpr const Colour PLUM { 0.867F, 0.627F, 0.867F };                 /**< PLUM { 0.867F, 0.627F, 0.867F }                 */
		constexpr const Colour POWDERBLUE { 0.690F, 0.878F, 0.902F };           /**< POWDERBLUE { 0.690F, 0.878F, 0.902F }           */
		constexpr const Colour PURPLE { 0.502F, 0.000F, 0.502F };               /**< PURPLE { 0.502F, 0.000F, 0.502F }               */
		constexpr const Colour RED { 1.000F, 0.000F, 0.000F };                  /**< RED { 1.000F, 0.000F, 0.000F }                  */
		constexpr const Colour ROSYBROWN { 0.737F, 0.561F, 0.561F };            /**< ROSYBROWN { 0.737F, 0.561F, 0.561F }             */
		constexpr const Colour ROYALBLUE { 0.255F, 0.412F, 0.882F };            /**< ROYALBLUE { 0.255F, 0.412F, 0.882F }            */
		constexpr const Colour SADDLEBROWN { 0.545F, 0.271F, 0.075F };          /**< SADDLEBROWN { 0.545F, 0.271F, 0.075F }          */
		constexpr const Colour SALMON { 0.980F, 0.502F, 0.447F };               /**< SALMON { 0.980F, 0.502F, 0.447F }               */
		constexpr const Colour SANDYBROWN { 0.957F, 0.643F, 0.376F };           /**< SANDYBROWN { 0.957F, 0.643F, 0.376F }           */
		constexpr const Colour SEAGREEN { 0.180F, 0.545F, 0.341F };             /**< SEAGREEN { 0.180F, 0.545F, 0.341F }             */
		constexpr const Colour SEASHELL { 1.000F, 0.961F, 0.933F };             /**< SEASHELL { 1.000F, 0.961F, 0.933F }             */
		constexpr const Colour SIENNA { 0.627F, 0.322F, 0.176F };               /**< SIENNA { 0.627F, 0.322F, 0.176F }               */
		constexpr const Colour SILVER { 0.753F, 0.753F, 0.753F };               /**< SILVER { 0.753F, 0.753F, 0.753F }               */
		constexpr const Colour SKYBLUE { 0.529F, 0.808F, 0.922F };              /**< SKYBLUE { 0.529F, 0.808F, 0.922F }             */
		constexpr const Colour SLATEBLUE { 0.416F, 0.353F, 0.804F };            /**< SLATEBLUE { 0.416F, 0.353F, 0.804F }            */
		constexpr const Colour SLATEGRAY { 0.439F, 0.502F, 0.565F };            /**< SLATEGRAY { 0.439F, 0.502F, 0.565F }            */
		constexpr const Colour SLATEGREY { 0.439F, 0.502F, 0.565F };            /**< SLATEGREY { 0.439F, 0.502F, 0.565F }            */
		constexpr const Colour SNOW { 1.000F, 0.980F, 0.980F };                 /**< SNOW { 1.000F, 0.980F, 0.980F }                 */
		constexpr const Colour SPRINGGREEN { 0.000F, 1.000F, 0.498F };          /**< SPRINGGREEN { 0.000F, 1.000F, 0.498F }          */
		constexpr const Colour STEELBLUE { 0.275F, 0.510F, 0.706F };            /**< STEELBLUE { 0.275F, 0.510F, 0.706F }            */
		constexpr const Colour TAN { 0.824F, 0.706F, 0.549F };                  /**< TAN { 0.824F, 0.706F, 0.549F }                  */
		constexpr const Colour TEAL { 0.000F, 0.502F, 0.502F };                 /**< TEAL { 0.000F, 0.502F, 0.502F }                 */
		constexpr const Colour THISTLE { 0.847F, 0.749F, 0.847F };              /**< THISTLE { 0.847F, 0.749F, 0.847F }              */
		constexpr const Colour TOMATO { 1.000F, 0.388F, 0.278F };               /**< TOMATO { 1.000F, 0.388F, 0.278F }               */
		constexpr const Colour TURQUOISE { 0.251F, 0.878F, 0.816F };            /**< TURQUOISE { 0.251F, 0.878F, 0.816F }            */
		constexpr const Colour VIOLET { 0.933F, 0.510F, 0.933F };               /**< VIOLET { 0.933F, 0.510F, 0.933F }               */
		constexpr const Colour WHEAT { 0.961F, 0.871F, 0.702F };                /**< WHEAT { 0.961F, 0.871F, 0.702F }                */
		constexpr const Colour WHITE { 1.000F, 1.000F, 1.000F };                /**< WHITE { 1.000F, 1.000F, 1.000F }                */
		constexpr const Colour WHITESMOKE { 0.961F, 0.961F, 0.961F };           /**< WHITESMOKE { 0.961F, 0.961F, 0.961F }           */
		constexpr const Colour YELLOW { 1.000F, 1.000F, 0.000F };               /**< YELLOW { 1.000F, 1.000F, 0.000F }               */
		constexpr const Colour YELLOWGREEN { 0.604F, 0.804F, 0.196F };          /**< YELLOWGREEN { 0.604F, 0.804F, 0.196F }          */
	}  // namespace COLOURS
}  // namespace ASGE
