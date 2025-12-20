#include <grpcpp/grpcpp.h>
#include "proto/a.grpc.pb.h"
#include "proto/a.pb.h"

class ProcessingImpl : public ProcessingServices::Service{
    ::grpc::Status computeSum(::grpc::ServerContext* context, const::Point3* request, ::Numeric* response){
        std::cout<<"CALLED!"<<std::endl;
        response->set_value(request->x() + request->y() + request->z());
        return grpc::Status::OK;
    }
};

int main(){
    ProcessingImpl service;
    grpc::ServerBuilder builder;
    builder.AddListeningPort("0.0.0.0:9999", grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

    server->Wait();

    return 0;
}