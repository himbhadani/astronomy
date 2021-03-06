#ifndef BOOST_ASTRONOMY_COORDINATE_SKY_POINT_HPP
#define BOOST_ASTRONOMY_COORDINATE_SKY_POINT_HPP


#include <string>
#include <type_traits>
#include <cmath>

#include <boost/units/systems/angle/degrees.hpp>
#include <boost/units/io.hpp>
#include <boost/static_assert.hpp>

#include <boost/astronomy/coordinate/frame.hpp>
#include <boost/astronomy/detail/is_base_template_of.hpp>


namespace boost
{
    namespace astronomy
    {
        namespace coordinate
        {
            //typedef boost::units::quantity<boost::units::degree::plane_angle> degree;

            /*sky_point is used to represent a point(coordinate) in the sky*/
            template <typename System>
            struct sky_point
            {
                ///@cond INTERNAL
                BOOST_STATIC_ASSERT_MSG((boost::astronomy::detail::is_base_template_of
                    <boost::astronomy::coordinate::base_frame, System>::value),
                    "Template argument is expected to be a fram class");
                ///@endcond
                
            protected:
                System point;

            public:
                //constructors  

                //default constructor
                sky_point() {}

                sky_point(System const& object) : point(object) {}

                //create point with providing representation and differential class object
                template <typename Representation, typename Differential>
                sky_point(Representation const& point, Differential const& diff)
                {
                    BOOST_STATIC_ASSERT_MSG((boost::astronomy::detail::is_base_template_of
                        <boost::astronomy::coordinate::base_representation, Representation>::value),
                        "argument type is expected to be a representation class");
                    this->data = representation_data;

                    BOOST_STATIC_ASSERT_MSG((boost::astronomy::detail::is_base_template_of
                        <boost::astronomy::coordinate::base_differential, Differential>::value),
                        "argument type is expected to be a differential class");

                    point = System(point, diff);
                }

                //create point with direct values of representation and differential
                sky_point(double lat, double lon, double distance, double pm_lat, double pm_lon_coslat, double radial_velocity)
                {
                    point = System(lat, lon, distance, pm_lat, pm_lon_coslat, radian_velocity);
                }

                template<class OtherSystem>
                sky_point(const sky_point<OtherSystem> object);

                //constructing from direct value of representation
                sky_point(double lat, double lon, double distance=1)
                {
                    System temp(lat, lon, distance);
                    this->point = temp;
                }

                //constructing from name of object if available in the calatoge
                sky_point(std::string const& name);

                std::string get_constillation();

                sky_point<System> from_name(std::string const& name);

                double separation(const sky_point<System> object)
                {
                    return this->point.separation(object.get_point());
                }

                // Returns positional angle in the radian
                double positional_angle(const sky_point<System> object)
                {
                    boost::astronomy::coordinate::spherical_representation<boost::astronomy::coordinate::radian> 
                        p1(this->point.get_data()), p2(object.get_point().get_data());

                    double diff = p2.get_lon() - p1.get_lon();
                    double coslat = std::cos(p2.get_lat());

                    double x = std::sin(p2.get_lat()) * std::cos(p1.get_lat()) - coslat * std::sin(p1.get_lat()) * std::cos(diff);
                    double y = std::sin(diff) * coslat;

                    return std::atan2(x, y);
                }

                template<class OtherSystem>
                bool is_equivalant_system(const sky_point<OtherSystem> object)
                {
                    return std::is_same<System, otherSystem>::value;
                }

                template<class OtherSystem>
                sky_point<OtherSystem> transform_to();

                System get_point() const
                {
                    return this->point;
                }

                void set_point(System otherPoint)
                {
                    this->point = otherPoint;
                }

            }; //sky_point

        } // namespace coordinate
    } // namespace astronomy
} // namespace boost

#endif // !BOOST_ASTRONOMY_COORDINATE_SKY_POINT_HPP