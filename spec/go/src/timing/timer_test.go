package timing

import (
	"github.com/stretchr/testify/assert"
	"testing"
	"time"
)

func TestTimer(t *testing.T) {
	name := "timer"
	timer := NewTimer(name, NewClock("clock"), -1)
	assert.NotNil(t, timer)
	assert.Contains(t, timer.String(), name)
}

func startWait(t *testing.T, name string) {
	timer := NewTimer(name, NewClock("clock"), -1)
	assert.NotNil(t, timer)
	assert.Contains(t, timer.String(), name)

	time.Sleep(200 * time.Millisecond)
	assert.True(t, timer.ElapsedTime() >= .15)
	assert.False(t, timer.Expired())
}

func startWait2(t *testing.T, name string) {
	timer := NewTimer(name, NewClock("clock"), .3)
	time.Sleep(100 * time.Millisecond)
	assert.False(t, timer.Expired())
	time.Sleep(250 * time.Millisecond)
	assert.True(t, timer.Expired())
}

func TestTimerElapsedTimeExpired(t *testing.T) {
	startWait(t, "timer")
	startWait2(t, "timer")
}

func startRemainingTime(t *testing.T, name string) {
	timer := NewTimer(name, NewClock("clock"), 1)
	time.Sleep(100 * time.Millisecond)
	assert.True(t, timer.RemainingTime() < 1.0)
	assert.True(t, timer.RemainingTime() > .5)
	time.Sleep(100 * time.Millisecond)
	assert.True(t, timer.RemainingTime() < .8)
	assert.True(t, timer.RemainingTime() > .4)
	time.Sleep(800 * time.Millisecond)
	assert.True(t, timer.Expired())
	assert.Equal(t, 0.0, timer.RemainingTime())
}

func TestTimerRemainingTime(t *testing.T) {
	startRemainingTime(t, "timer")
}
