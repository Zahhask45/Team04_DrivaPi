#ifndef KUKSAREADER_HPP
#define KUKSAREADER_HPP

#include <QObject>
#include <grpcpp/grpcpp.h>
#include "val.grpc.pb.h"

// KUKSA Val gRPC namespace to avoid long names
using namespace kuksa::val::v1;

class KUKSAReader : public QObject
{
    Q_OBJECT
public:

private:
};

#endif // KUKSAREADER_HPP