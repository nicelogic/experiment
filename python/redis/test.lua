-- local a = redis.call('keys', 'sfu/room*')
-- for i, key in ipairs(a) do
--     redis.call('del', key)
-- end
-- local b = redis.call('smembers', 'sfu/worker/1_1572956304/routers')
-- for i, key in ipairs(b) do
--     redis.call('srem', 'sfu/worker/1_1572956304/routers', key)
-- end
--[[ type
local workersRedisKeyType = redis.call('type', 'aaa')
if workersRedisKeyType['ok'] == 'hash' then
redis.call('del', 'aaa')
return 'true'
end
return workersRedisKeyType
--]] -- for i = 1, 48, 1 do
--     redis.call('sadd', "sfuclient/034fa748-85d8-11ea-a8be-00163e0c7ccf/worker/035c8ca6-85d8-11ea-a8be-00163e0c7ccf_3/room/0000138/peers", i)
-- end
-- if(workersRedisKeyType == 'hash') then
--     redis.call('del', 'aaa')
--     return "del success"
-- end
-- return 'del false'
-- local function RedisHget(key, field)
--     local value = redis.call('hget', key, field)
--     if (value == false) then
--         return ''
--     else
--         return value
--     end
-- end
-- ----------------------------
-- local roomId = ARGV[1]
-- local log = ''
-- local roomLoadedMtNum = 0;
-- local roomRemainMtNum = 0;
-- local maxWorkerLoadMtNum = tonumber(redis.call('get', 'sfu/worker/mtlimit'))
-- if maxWorkerLoadMtNum == nil then maxWorkerLoadMtNum = 48 end
-- local roomWorkers = redis.call('smembers', 'sfu/workers/' .. roomId)
-- local roomClients = redis.call('hkeys', 'sfu/clients/' .. roomId)
-- local confs = redis.call('smembers', 'confs')
-- local roomTWorkers = {}
-- for key, workerId in ipairs(roomWorkers) do
--     local workerType = RedisHget('sfu/worker/' .. workerId, 'type')
--     if (workerType == 'terminal') then
--         roomTWorkers[#roomTWorkers + 1] = workerId
--     end
-- end
-- local roomTWorkersNum = #roomTWorkers
-- local roomTotalLoadWorkerNum = roomTWorkersNum * maxWorkerLoadMtNum
-- for key, workerId in ipairs(roomTWorkers) do
--     for key, clientID in ipairs(roomClients) do
--         for key, confE164 in ipairs(confs) do
--             local peerKey = 'sfuclient/' .. clientID .. '/worker/' ..
--                                 workerId .. '/room/' .. confE164 .. '/peers'
--             local workerConfPeersNum = redis.call('SCARD', peerKey)
--             roomLoadedMtNum = roomLoadedMtNum + workerConfPeersNum
--             if (workerConfPeersNum ~= 0) then
--                 log = log .. peerKey .. ' has: ' .. workerConfPeersNum ..
--                             '\n'
--             end
--         end
--     end
-- end
-- local roomLoadMtInfo = {
--     ['room_id'] = roomId,
--     ['loaded_mtnum'] = roomLoadedMtNum,
--     ['remain_mtnum'] = roomTotalLoadWorkerNum - roomLoadedMtNum,
--     ['log'] = log
-- }
-- -- return cjson.encode(roomLoadMtInfo)
-- local a = nil
-- local b = tonumber(nil)
-- if b == nil then b = 48 end
-- return b
-- local clients = redis.call('hgetall', 'sfu/clients')
-- for i = 1, #clients, 2 do
--    redis.call('hset', 'sfu/clients/mooooooo-oooo-oooo-oooo-defaultmachi', clients[i], clients[i+1]) 
-- end



-- local function Fun(obj)
--     obj['hi'] = 'aaa'
-- end
-- local obj = {['a']='b'}
-- Fun(obj)
-- return cjson.encode(obj)

redis.call('hset', 'aaa', 'name', 1)
local b = redis.call('hget', 'aaa', 'name')
redis.call('hset', 'bbb', 'name', b)
redis.log(redis.LOG_WARNING, 'tttttest')

return b