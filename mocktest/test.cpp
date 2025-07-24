#include "gmock/gmock.h"
#include "booking_scheduler.cpp"

TEST(BookingSchedulerTest, 예약은정시에만가능하다정시가아닌경우예약불가) {
	//arrange
	tm notontheHour{ 0 };
	notontheHour.tm_year = 2021 - 1900;
	notontheHour.tm_mon = 03 - 1;
	notontheHour.tm_mday = 26;
	notontheHour.tm_hour = 9;
	notontheHour.tm_min =5;
	notontheHour.tm_isdst = - 1;
	mktime(&notontheHour);
	Customer customer{ "Fake name ", "010-1234-5678" };
	Schedule* schedule = new Schedule{ notontheHour, 1, customer };
	BookingScheduler bookingScheduler{ 3 };
	EXPECT_THROW({ bookingScheduler.addSchedule(schedule); }, std::runtime_error);

}

TEST(BookingSchedulerTest, 예약은정시에만가능하다정시인경우예약가능) {
	tm onntheHour{ 0 };
	onntheHour.tm_year = 2021 - 1900;
	onntheHour.tm_mon = 03 - 1;
	onntheHour.tm_mday = 26;
	onntheHour.tm_hour = 9;
	onntheHour.tm_min = 0;
	onntheHour.tm_isdst = -1;
	mktime(&onntheHour);
	Customer customer{ "Fake name ", "010-1234-5678" };
	Schedule* schedule = new Schedule{ onntheHour, 1, customer };
	BookingScheduler bookingScheduler{ 3 };
	//ACT
	bookingScheduler.addSchedule(schedule);
	EXPECT_EQ(true, bookingScheduler.hasSchedule(schedule));

}

TEST(BookingSchedulerTest, 시간대별인원제한이있다같은시간대에Capacity초과할경우예외발생) {

}

TEST(BookingSchedulerTest, 시간대별인원제한이있다같은시간대가다르면Capacity차있어도스케쥴추가성공) {

}

TEST(BookingSchedulerTest, 예약완료시SMS는무조건발송) {

}

TEST(BookingSchedulerTest, 이메일이없는경우에는이메일미발송) {

}

TEST(BookingSchedulerTest, 이메일이있는경우에는이메일발송) {

}

TEST(BookingSchedulerTest, 현재날짜가일요일인경우예약불가예외처리) {

}

TEST(BookingSchedulerTest, 현재날짜가일요일이아닌경우예약가능) {

}

int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}