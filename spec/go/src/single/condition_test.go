package single

import (
	"github.com/NeoResearch/libbft/src/timing"
	"github.com/stretchr/testify/assert"
	"testing"
)

func TestCondition(t *testing.T) {
	f := func(t timing.Timer, param Param, me int) bool {
		return me%2 == 0
	}
	condition := NewCondition("teste", f)
	assert.NotNil(t, condition)
	timer := timing.NewDefaultTimer()

	assert.Equal(t, f(timer, "", 1), condition.GetTimedFunction()(timer, "", 1))

	for i := 0; i < 10; i++ {
		assert.Equal(t, f(timer, "", i), condition.GetTimedFunction()(timer, "", i))
	}
}
