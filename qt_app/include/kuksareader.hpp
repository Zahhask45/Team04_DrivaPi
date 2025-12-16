#ifndef KUKSAREADER_HPP
#define KUKSAREADER_HPP

#include <QObject>
#include <QDebug>
#include <grpcpp/grpcpp.h>
#include "val.grpc.pb.h"

// KUKSA Val gRPC namespace to avoid long names
using namespace kuksa::val::v1;

class KUKSAReader : public QObject
{
    Q_OBJECT
public:
    explicit KUKSAReader(QObject *parent = nullptr);
    ~KUKSAReader() override;
public slots:
    // Starts the data fetching process
    void start();

signals:
    // Emitted when new speed data is received
    void speedReceived(float speed);

private:
    // gRPC client stub -> used to communicate with KUKSA Val server
    std::unique_ptr<VAL::Stub> m_stub_;
};

#endif // KUKSAREADER_HPP