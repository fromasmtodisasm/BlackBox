-- : ./methoddispatcher.lua
MethodDispatcherPool={
	_pool={},
};

function MethodDispatcherPool:AddObj(obj,idx)
	local _idx=obj;
	if(idx)then
		_idx=idx;
	end
	self._pool[_idx]=obj;
end

function MethodDispatcherPool:RemoveObject(idx)
	self._pool[idx]=nil;
end

function MethodDispatcherPool:Exists(idx)
	return self._pool[idx];
end

function MethodDispatcherPool.IndexHandler(table,index)
	local f = function(...)
		-- FIXME: this is hack
		if false then
		local pool=table._pool
		for i,val in pairs(pool) do
			local pool_func=val[index];
			if(pool_func)then
				arg[1]=val;
				call(pool_func,arg);
			end
		end
		end
	end
	table[index]=f;
	return f;
end

function MethodDispatcherPool.new()
    local mmp = {}
    setmetatable(mmp, {
        __index = MethodDispatcherPool.IndexHandler  -- ”казываем, что использовать дл€ несуществующих ключей
    })
    return mmp
end
