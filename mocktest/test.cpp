#include "gmock/gmock.h"
#include "booking_scheduler.cpp"
#include "testable_sms_sender.cpp"
using namespace testing;
class BookingItem : public Test {
protected:
	void SetUp() override {
		NON_ON_TIME_HOUR = getTime(2021, 3, 26, 9, 5);
		ON_TIME_HOUR = getTime(2021, 3, 26, 9, 0);
		bookingScheduler.setSmsSender(&testablesmsSender);

	}
public:
	
	tm getTime(int year, int mon, int mday, int hour, int min) {
		tm result = { 0,min,hour,mday, mon - 1,year - 1900, 0,0,-1 };
		mktime(&result);
		return result;
	}	
	tm plusTime(tm base, int hour) {
		base.tm_hour += hour;
		mktime(&base);
		return base;
	}
	tm NON_ON_TIME_HOUR;
	tm ON_TIME_HOUR;

	Customer CUSTOMER{ "Fake name ", "010-1234-5678" };
		const int UNDER_CAPA = 1;
	const int CAPA_PER_HOUR = 3;

	BookingScheduler bookingScheduler{ CAPA_PER_HOUR };
	TestableSmsSender testablesmsSender;
private:

};
TEST_F(BookingItem, 예약은정시에만가능하다정시가아닌경우예약불가) {
	//arrange
	Schedule* schedule = new Schedule{ NON_ON_TIME_HOUR, UNDER_CAPA, CUSTOMER };
	EXPECT_THROW({ bookingScheduler.addSchedule(schedule); }, std::runtime_error);

}

TEST_F(BookingItem, 예약은정시에만가능하다정시인경우예약가능) {
	Schedule* schedule = new Schedule{ ON_TIME_HOUR, UNDER_CAPA, CUSTOMER };
	//ACT
	bookingScheduler.addSchedule(schedule);
	EXPECT_EQ(true, bookingScheduler.hasSchedule(schedule));

}

TEST_F(BookingItem, 시간대별인원제한이있다같은시간대에Capacity초과할경우예외발생) {
	Schedule* schedule = new Schedule{ ON_TIME_HOUR, CAPA_PER_HOUR, CUSTOMER };
	bookingScheduler.addSchedule(schedule);
	try
	{
		Schedule* newSchedule = new Schedule{ ON_TIME_HOUR, UNDER_CAPA, CUSTOMER };
		bookingScheduler.addSchedule(newSchedule);
		FAIL();
	}
	catch (std::runtime_error & e)
	{
		EXPECT_EQ(string{ e.what() }, string{ "Number of people is over restaurant capacity per hour" });
	}

}

TEST_F(BookingItem, 시간대별인원제한이있다같은시간대가다르면Capacity차있어도스케쥴추가성공) {
	Schedule* schedule = new Schedule{ ON_TIME_HOUR, CAPA_PER_HOUR, CUSTOMER };
	bookingScheduler.addSchedule(schedule);
	
	Schedule* newschedule = new Schedule{ plusTime(ON_TIME_HOUR,1), UNDER_CAPA, CUSTOMER};
	bookingScheduler.addSchedule(newschedule);

	EXPECT_EQ(true, bookingScheduler.hasSchedule(newschedule));

}

TEST_F(BookingItem, 예약완료시SMS는무조건발송) {
	Schedule* schedule = new Schedule{ ON_TIME_HOUR, CAPA_PER_HOUR, CUSTOMER };
	bookingScheduler.addSchedule(schedule);
	EXPECT_EQ(true, testablesmsSender.isSendMethodIsCallded());
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