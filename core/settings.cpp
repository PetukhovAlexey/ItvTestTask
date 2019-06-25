#include <core/settings.h>
#include <thread>

const int core::def::server_port(19868);
const int core::def::jpeg_quality(65);
const int core::def::max_requests_in_work(20);
const int core::def::socket_buffer_size(1024);
const int core::def::maximum_message_size(0x12d0000); //16Mb
const int core::def::threads_in_pool(std::thread::hardware_concurrency() + 1);
const std::string core::def::address("localhost");
const std::string core::def::input_jpeg_file("input.jpg");
const std::string core::def::output_jpeg_file("output.jpg");
const std::string core::def::text_for_image("text for image");
const int core::def::delay_seconds(3);
