--dynamic zombie creating based on how much killed?
--time is in seconds elapsed?
--difficulty 1/2/3   easy/med/hard
gameTable = {difficulty = 1, time = 60}
--time is to be updated whenever you create a scaling zombie, in seconds
gameTable.worldWidth = 700
gameTable.worldHeight = 700
gameTable.maxSpeed = 100.0
--stats i need to know to create a zombie according to players current upgrade, to be updated before zombie creation
gameTable.chuckerHP = 7
gameTable.chuckerAttack = 1
gameTable.chuckerAttackSpeed = 1.5
gameTable.chuckerRange = 100

gameTable.stinkieHP = 10
gameTable.stinkieAttack = 2
gameTable.stinkieAttackSpeed = 2.0
gameTable.stinkieRange = 50

gameTable.holyBoneHP = 5
gameTable.holyBoneAttack = 0
gameTable.holyBoneAttackSpeed = 3.0
gameTable.holyBoneRange = 100
gameTable.holyBoneHeal = 1



--stats for basic zombie attributes
gameTable.basicChuckerHP = 7
gameTable.basicChuckerAttack = 1
gameTable.basicChuckerAttackSpeed = 1.5
gameTable.basicChuckerRange = 100

gameTable.basicStinkieHP = 10
gameTable.basicStinkieAttack = 2
gameTable.basicStinkieAttackSpeed = 2.0
gameTable.basicStinkieRange = 50

gameTable.basicHolyBoneHP = 5
gameTable.basicHolyBoneAttack = 0
gameTable.basicHolyBoneAttackSpeed = 3.0
gameTable.basicHolyBoneRange = 100
gameTable.basicHolyBoneHeal = 1



function getTable()
  generalTable = {}
  
  
  --basic components
  generalTable.ChasingComp = false
  generalTable.SeperationComp = false
  generalTable.FollowingComp = false
  generalTable.MeleeAttackComp = false
  generalTable.KeyboardComp = false
  
  --pooled components
  generalTable.ActionFlagComp = false
  generalTable.AnimComp = false
  generalTable.AnimCompName = nil
  generalTable.CombatComp = false
  generalTable.DomainComp = false
  generalTable.DomainCompRadius = nil
  generalTable.HealComp = false
  generalTable.HordeStatusComp = false
  generalTable.KineticComp = false
  generalTable.KineticCompMaxSpeed = nil
  generalTable.ZombieSensorComp = false
  
  -- combat statistics
  generalTable.hp = nil
  generalTable.attack = nil
  generalTable.attackSpeed = nil
  generalTable.range = nil
  generalTable.heal = nil
  
  --others
  generalTable.ZombieCatagory = nil
  generalTable.isPlayer = false
  generalTable.StinkieNum = 0
  generalTable.ChuckerNum = 0
  generalTable.HolyBoneNum = 0
  
  --functions
  generalTable.StinkieFunc = nil
  generalTable.ChuckerFunc = nil
  generalTable.HolyFunc = nil
  
  
  return generalTable
end

function createStinkie(HP, ATTACK, ATTACKSPEED, RANGE)
  table = getTable()
  table = {hp = HP, attack = ATTACK, attackSpeed = ATTACKSPEED, range =RANGE}
  --basic components
  table.ChasingComp = true
  table.SeperationComp = true
  table.FollowingComp = true
  table.MeleeAttackComp = true
  
  --pooled Components
  table.AnimComp = true
  table.AnimCompName = "basic_zombie"
  table.ActionFlagComp = true
  table.KineticComp = true
  table.KineticCompMaxSpeed = gameTable.maxSpeed + math.random(-30,30)
  
  --others
  table.ZombieCatagory = 1
  return table
end

function createBasicStinkie()
	return createStinkie(gameTable.basicStinkieHP,gameTable.basicStinkieAttack,gameTable.basicStinkieAttackSpeed,gameTable.basicStinkieRange)
end

function createScalingStinkie()
  return createStinkie(10+(gameTable.time / 30)*gameTable.difficulty,  2+(gameTable.time / 60)*gameTable.difficulty, 2.0-(.001*gameTable.time*gameTable.difficulty),10)
end

function createChucker(HP, ATTACK, ATTACKSPEED, RANGE)
  table = getTable()
  table = {hp = HP, attack = ATTACK, attackSpeed = ATTACKSPEED, range =RANGE}
    --basic components
  table.ChasingComp = true
  table.SeperationComp = true
  table.FollowingComp = true
  
  --pooled Components
  table.AnimComp = true
  table.AnimCompName = "chucker"
  table.ActionFlagComp = true
  table.KineticComp = true
  table.KineticCompMaxSpeed = gameTable.maxSpeed + math.random(-30,30)
  
  --others
  table.ZombieCatagory = 3
  return table
end

function createBasicChucker()
	return createChucker(gameTable.basicChuckerHP,gameTable.basicChuckerAttack,gameTable.basicChuckerAttackSpeed,gameTable.basicChuckerRange)
end

function createScalingChucker( )
  return createChucker(7+(gameTable.time / 30)*gameTable.difficulty, 1+(gameTable.time / 60)*gameTable.difficulty, 1.5-(.001*gameTable.time*gameTable.difficulty), 100)
end



function createHolyBone(HP, ATTACK, ATTACKSPEED, RANGE, HEAL)
  table = getTable()
  table = {hp = HP, attack = ATTACK, attackSpeed = ATTACKSPEED, range =RANGE}
    --basic components
  table.ChasingComp = true
  table.SeperationComp = true
  table.FollowingComp = true
  
  --pooled Components
  table.AnimComp = true
  table.AnimCompName = "healer"
  table.ActionFlagComp = true
  table.HealComp = true
  table.KineticComp = true
  table.KineticCompMaxSpeed = gameTable.maxSpeed + math.random(-30,30)
  
  --others
  table.ZombieCatagory = 2
  return table
end

function createBasicHolyBone()
	return createHolyBone(gameTable.basicHolyBoneHP,gameTable.basicHolyBoneAttack,gameTable.basicHolyBoneAttackSpeed,gameTable.basicHolyBoneRange,gameTable.basicHolyBoneHeal)
end

function createScalingHolyBone()
  return createHolyBone(5+(gameTable.time / 30)*gameTable.difficulty,0,3-(.001*gameTable.time*gameTable.difficulty),100,1 +(gameTable.time / 120)*gameTable.difficulty)
end



function createPlayer(isHuman)
  player = getTable()
  player.isPlayer = true
  
  --pooled Components
  player.AnimComp = true
  player.KineticComp = true
  player.KineticCompMaxSpeed = 2 * gameTable.maxSpeed
  player.DomainComp = true
  player.DomainCompRadius = 50
  player.HordeStatusComp = true
  
  if (isHuman) then 
    player.KeyboardComp = true
    player.x = gameTable.worldWidth / 2
    player.y = gameTable.worldHeight / 2
    player.AnimCompName = "swiss"
    player.StinkieFunc = function ()
      return createStinkie(gameTable.stinkieHP,gameTable.stinkieAttack, gameTable.stinkieAttackSpeed,gameTable.stinkieRange) 
    end
    player.ChuckerFunc = function()
      return createChucker(gameTable.chuckerHP,gameTable.chuckerAttack,gameTable.chuckerAttackSpeed,gameTable.chuckerRange)
    end
    player.HolyBoneFunc = function()
      return createHolyBone(gameTable.holyBoneHP,gameTable.holyBoneAttack,gameTable.holyBoneAttackSpeed,gameTable.holyBoneRange,gameTable.holyBoneHeal)
    end
    

  else 
    player.x = math.random(0,gameTable.worldWidth)
    player.y = math.random(0,gameTable.worldHeight)
    --to be changed to actual name for animation of other horde players
    player.AnimCompName = "otherSwiss"
    player.StinkieNum = 1+gameTable.difficulty*(gameTable.time/30)*1
    player.ChuckerNum = gameTable.difficulty*(gameTable.time/55)*1
    player.HolyBoneNum = gameTable.difficulty*(gameTable.time/80)*1
    player.StinkieFunc = function ()
      return createStinkie(10+(gameTable.time / 30)*gameTable.difficulty,  2+(gameTable.time / 60)*gameTable.difficulty, 2.0    -(.001*gameTable.time*gameTable.difficulty),10) 
    end
    player.ChuckerFunc = function()
      return createChucker(7+(gameTable.time / 30)*gameTable.difficulty, 1+(gameTable.time / 60)*gameTable.difficulty, 1.5-(    .001*gameTable.time*gameTable.difficulty), 100)
    end
    player.HolyBoneFunc = function()
      return createHolyBone(5+(gameTable.time / 30)*gameTable.difficulty,0,3-(.001*gameTable.time*gameTable.difficulty),100,    1 +(gameTable.time / 120)*gameTable.difficulty)
    end
    
    
    
    end
  
  return player
end
player = createPlayer(true)
basicChucker = createBasicChucker()
scalingChucker = player.ChuckerFunc()
print(basicChucker.hp,scalingChucker.hp)
print(basicChucker.attack,scalingChucker.attack)
print(basicChucker.attackSpeed,scalingChucker.attackSpeed)
print(basicChucker.heal,scalingChucker.heal)
print(basicChucker.DomainComp,scalingChucker.DomainCompRadius)




