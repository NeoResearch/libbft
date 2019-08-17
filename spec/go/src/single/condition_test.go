package single

import (
	"github.com/NeoResearch/libbft/src/timing"
	"github.com/stretchr/testify/assert"
	"testing"
)

func TestCondition(t *testing.T) {
	f := func(t timing.Timer, param Param, me int) (bool, error) {
		return me%2 == 0, nil
	}
	condition := NewCondition("teste", f)
	assert.NotNil(t, condition)
	timer := timing.NewDefaultTimer()

	rfv, rfe := f(timer, "", 1)
	assert.Nil(t, rfe)
	rcv, rce := condition.GetTimedFunction()(timer, "", 1)
	assert.Nil(t, rce)
	assert.Equal(t, rfv, rcv)

	for i := 0; i < 10; i++ {
		rfv, rfe = f(timer, "", i)
		assert.Nil(t, rfe)
		rcv, rce = condition.GetTimedFunction()(timer, "", i)
		assert.Nil(t, rce)
		assert.Equal(t, rfv, rcv)
	}
}
