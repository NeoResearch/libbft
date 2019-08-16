package timing

import (
	"fmt"
	"time"
)

type CountdownNotifyTimer interface {
	// superclass
	GetCountdownTimer() CountdownTimer
	// get / set
	GetClock() Clock
	GetMyTime() time.Time
	GetName() string
	GetCountdown() float64
	// methods
	Init(countdown float64)
	Reset()
	Expired() bool

	String() string
}

type CountdownNotifyTimerService struct {
	countdownTimer CountdownTimer
}

func NewCountdownNotifyTimer(countdown float64, clock Clock, name string, myTime float64) CountdownNotifyTimer {
	return &CountdownNotifyTimerService {
		NewCountdownTimer(countdown, clock, name, myTime),
	}
}

func (c *CountdownNotifyTimerService) GetCountdownTimer() CountdownTimer {
	return c.countdownTimer
}

func (c *CountdownNotifyTimerService) GetClock() Clock {
	return c.GetCountdownTimer().GetClock()
}

func (c *CountdownNotifyTimerService) GetMyTime() time.Time {
	return c.GetCountdownTimer().GetMyTime()
}

func (c *CountdownNotifyTimerService) GetName() string {
	return c.GetCountdownTimer().GetName()
}

func (c *CountdownNotifyTimerService) GetCountdown() float64 {
	return c.GetCountdownTimer().GetCountdown()
}

func (c *CountdownNotifyTimerService) Init(countdown float64) {
	c.GetCountdownTimer().Init(countdown)
}

func (c *CountdownNotifyTimerService) Reset() {
	c.GetCountdownTimer().Reset()
}

func (c *CountdownNotifyTimerService) Expired() bool {
	return c.GetCountdownTimer().Expired()
}

func (c *CountdownNotifyTimerService) String() string {
	return fmt.Sprintf("CountdownNotifyTimer {name=%v}", c.GetName())
}
