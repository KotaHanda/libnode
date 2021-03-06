// Copyright (c) 2012 Plenluno All rights reserved.

#include <gtest/gtest.h>
#include <libnode/http_status.h>

namespace libj {
namespace node {
namespace http {

TEST(GTestHttpStatus, TestCreate) {
    Status::CPtr s = Status::create(Status::OK);
    ASSERT_TRUE(s);
    s = Status::create(libj::Status::OK);
    ASSERT_FALSE(s);
}

TEST(GTestHttpStatus, TestMessage) {
    String::CPtr notFound = String::create("Not Found");
    Status::CPtr s = Status::create(Status::NOT_FOUND);
    ASSERT_EQ(s->message()->compareTo(notFound), 0);
}

TEST(GTestHttpStatus, TestToString) {
    String::CPtr notFound = String::create("Not Found");
    Status::CPtr s = Status::create(Status::NOT_FOUND);
    ASSERT_EQ(s->toString()->compareTo(notFound), 0);
}

TEST(GTestHttpStatus, TestInstanceOf) {
    Status::CPtr s = Status::create(Status::NOT_FOUND);
    ASSERT_TRUE(s->instanceof(Type<Status>::id()));
    ASSERT_TRUE(s->instanceof(Type<libj::Status>::id()));
    ASSERT_TRUE(s->instanceof(Type<Immutable>::id()));
    ASSERT_TRUE(s->instanceof(Type<Object>::id()));
}

}  // namespace http
}  // namespace node
}  // namespace libj
