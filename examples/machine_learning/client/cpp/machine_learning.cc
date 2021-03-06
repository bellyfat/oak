/*
 * Copyright 2019 The Project Oak Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "examples/machine_learning/proto/machine_learning.grpc.pb.h"
#include "examples/machine_learning/proto/machine_learning.pb.h"
#include "glog/logging.h"
#include "include/grpcpp/grpcpp.h"
#include "oak/client/application_client.h"

ABSL_FLAG(std::string, address, "localhost:8080", "Address of the Oak application to connect to");
ABSL_FLAG(std::string, ca_cert, "", "Path to the PEM-encoded CA root certificate");

using ::oak::examples::machine_learning::MachineLearning;
using ::oak::examples::machine_learning::MLData;
using ::oak::examples::machine_learning::MLLearn;
using ::oak::examples::machine_learning::MLPredict;
using ::oak::examples::machine_learning::MLResponse;

std::string send_data(MachineLearning::Stub* stub) {
  ::grpc::ClientContext context;
  MLData data;
  // FIXME: set data here
  MLResponse response;
  ::grpc::Status status = stub->Data(&context, data, &response);
  if (!status.ok()) {
    LOG(FATAL) << "Could not submit data: " << oak::status_code_to_string(status.error_code())
               << ": " << status.error_message();
  }
  return response.message();
}

std::string learn(MachineLearning::Stub* stub) {
  ::grpc::ClientContext context;
  MLLearn learn;
  MLResponse response;
  ::grpc::Status status = stub->Learn(&context, learn, &response);
  if (!status.ok()) {
    LOG(FATAL) << "Could not learn: " << oak::status_code_to_string(status.error_code()) << ": "
               << status.error_message();
  }
  return response.message();
}

std::string predict(MachineLearning::Stub* stub) {
  ::grpc::ClientContext context;
  MLPredict predict;
  MLResponse response;
  ::grpc::Status status = stub->Predict(&context, predict, &response);
  if (!status.ok()) {
    LOG(FATAL) << "Could not predict: " << oak::status_code_to_string(status.error_code()) << ": "
               << status.error_message();
  }
  return response.message();
}

int main(int argc, char** argv) {
  absl::ParseCommandLine(argc, argv);

  std::string address = absl::GetFlag(FLAGS_address);
  std::string ca_cert = oak::ApplicationClient::LoadRootCert(absl::GetFlag(FLAGS_ca_cert));
  LOG(INFO) << "Connecting to Oak Application: " << address;

  // TODO(#1066): Use a more restrictive Label.
  oak::label::Label label = oak::PublicUntrustedLabel();
  // Connect to the Oak Application.
  auto stub = MachineLearning::NewStub(oak::ApplicationClient::CreateChannel(
      address, oak::ApplicationClient::GetTlsChannelCredentials(ca_cert), label));
  if (stub == nullptr) {
    LOG(FATAL) << "Failed to create application stub";
  }

  // Perform multiple invocations of the same Oak Application, with different parameters.
  auto message_0 = send_data(stub.get());
  LOG(INFO) << "data response: " << message_0;

  auto message_1 = learn(stub.get());
  LOG(INFO) << "learn response: " << message_1;

  auto message_2 = predict(stub.get());
  LOG(INFO) << "predict response: " << message_2;

  return EXIT_SUCCESS;
}
