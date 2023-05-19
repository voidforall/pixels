#include <iostream>
#include <gtest/gtest.h>

#include "duckdb.hpp"
#include "grpc/transpile_sql_client.h"
#include "yaml-cpp/yaml.h"

YAML::Node config = YAML::LoadFile("../../config.yaml");

TEST(grpc, transpileTest) {
    std::string host_addr = config["server_address"].as<std::string>();
    std::string port = config["server_port"].as<std::string>();
    TranspileSqlClient client(grpc::CreateChannel(host_addr + ":" + port, grpc::InsecureChannelCredentials()));

    std::string token = "";
    std::string sql_statement = "SELECT STRFTIME(x, '%y-%-m-%S');";
    std::string from_dialect = "duckdb";
    std::string to_dialect = "hive";
    std::string expected_sql = "SELECT DATE_FORMAT(x, 'yy-M-ss')";

    std::string transpiled_sql = client.TranspileSql(token, sql_statement, from_dialect, to_dialect);
    EXPECT_EQ(transpiled_sql, expected_sql);
}