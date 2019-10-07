/**
 * @file Helper.tpp
 * @authors Giulio Romualdi <giulio.romualdi@iit.it>
 * @copyright 2018 iCub Facility - Istituto Italiano di Tecnologia
 *            Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 * @date 2018
 */

// std
#include <iostream>

// YARP
#include <yarp/os/LogStream.h>

template <typename T>
bool WalkingControllers::YarpHelper::yarpListToVector(const yarp::os::Value& input, T& output)
{
    if (input.isNull())
    {
        yError() << "[yarpListToVector] Empty input value.";
        return false;
    }
    if (!input.isList() || !input.asList())
    {
        yError() << "[yarpListToVector] Unable to read the input list.";
        return false;
    }
    yarp::os::Bottle *inputPtr = input.asList();

    if (inputPtr->size() != output.size())
    {
        yError() << "[yarpListToVector] The dimension set in the configuration file is not "
                 << output.size();
        return false;
    }

    for (int i = 0; i < inputPtr->size(); i++)
    {
        if (!inputPtr->get(i).isDouble() && !inputPtr->get(i).isInt())
        {
            yError() << "[yarpListToVector] The input is expected to be a double or an int";
            return false;
        }
        output(i) = inputPtr->get(i).asDouble();
    }
    return true;
}

template <typename  T>
bool WalkingControllers::YarpHelper::getVectorFromSearchable(const yarp::os::Searchable& config,
                                         const std::string& key,
                                         T& vector)
{
    yarp::os::Value* value;
    if(!config.check(key, value))
    {
        yError() << "[getiDynTreeVectorFixSizeFromSearchable] Missing field "<< key;
        return false;
    }

    return yarpListToVector(*value, vector);
}

template <typename T>
void WalkingControllers::YarpHelper::mergeSigVector(yarp::sig::Vector& vector, const T& t)
{
    for(int i= 0; i<t.size(); i++)
        vector.push_back(t(i));

    return;
}

template <typename T, typename... Args>
void WalkingControllers::YarpHelper::mergeSigVector(yarp::sig::Vector& vector, const T& t, const Args&... args)
{
    for(int i= 0; i<t.size(); i++)
        vector.push_back(t(i));

    mergeSigVector(vector, args...);

    return;
}

template <typename... Args>
void WalkingControllers::YarpHelper::sendVariadicVector(yarp::os::BufferedPort<yarp::sig::Vector>& port, const Args&... args)
{
    yarp::sig::Vector& vector = port.prepare();
    vector.clear();

    mergeSigVector(vector, args...);

    port.write();
}
