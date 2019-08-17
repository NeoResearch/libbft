package single

import (
	"github.com/NeoResearch/libbft/src/timing"
	"github.com/stretchr/testify/assert"
	"testing"
)

func TestAction(t *testing.T) {
	value := -1
	f := func(t timing.Timer, param Param, me int) {
		value = me
	}
	action := NewAction("teste", f)
	assert.NotNil(t, action)
	timer := timing.NewDefaultTimer()

	f(timer, "", 1)
	assert.Equal(t, 1, value)
	value = -1
	action.GetTimedAction()(timer, "", 1)
	assert.Equal(t, 1, value)

	for i := 0; i < 10; i++ {
		value = -1
		f(timer, "", i)
		assert.Equal(t, i, value)

		value = -1
		action.GetTimedAction()(timer, "", i)
		assert.Equal(t, i, value)
	}
}

type actionReference struct {
	age int
}

func TestActionReference(t *testing.T) {
	obj := actionReference{1}
	f := func(t timing.Timer, param Param, me int) {
		param.(*actionReference).age += 1
	}

	timer := timing.NewDefaultTimer()
	f(timer, &obj, 1)
	assert.Equal(t, 2, obj.age)
}
