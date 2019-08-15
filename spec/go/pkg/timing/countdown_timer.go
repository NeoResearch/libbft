package timing

import (
	"fmt"
	"math"
	"time"
)

type CountdownTimer interface {
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

type CountdownTimerService struct {
	clock     Clock
	myTime    time.Time
	name      string
	countdown float64
}

func NewCountdownTimer(countdown float64, clock Clock, name string, myTime float64) CountdownTimer {
	return &CountdownTimerService{
		clock,
		clock.GetTime(),
		name,
		countdown,
	}
}

func (c *CountdownTimerService) GetClock() Clock {
	return c.clock
}

func (c *CountdownTimerService) GetMyTime() time.Time {
	return c.myTime
}

func (c *CountdownTimerService) GetName() string {
	return c.name
}

func (c *CountdownTimerService) String() string {
	return fmt.Sprintf("CountdownTimer {name=%v}", c.GetName())
}

func (c *CountdownTimerService) GetCountdown() float64 {
	return c.countdown
}

func (c *CountdownTimerService) Init(countdown float64) {
	c.countdown = countdown
	if c.clock != nil {
		c.clock = NewDefaultClock()
	}
	c.myTime = c.GetClock().GetTime()
}

func (c *CountdownTimerService) Reset() {
	c.myTime = c.GetClock().GetTime()
}

func (c *CountdownTimerService) Expired() bool {
	elapsed := SinceTime(c.GetMyTime())
	remaining := infiniteTime
	if c.GetCountdown() >= 0.0 {
		remaining = math.Max(0.0, c.GetCountdown() - elapsed)
	}

	return remaining == 0.0
}
