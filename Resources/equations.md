# Equations
## 1. Weight Handler

$ W_B = $ Base Carry Weight (`Settings::uBaseWeightLimit`)\
$ W_S = $ Stamina Weight Bonus (`staminaBonus`)\
$ W_L = $ Level Weight Bonus (`levelBonus`)\
$ W_F = $ Final Calculated Carry Weight (`weightLimit`)

$ M_S = $ Stamina Weight Bonus Modifier (`Settings::fStaminaWeightMod`)\
$ M_L = $ Level Weight Bonus Modifier (`Settings::fLevelWeightMod`)\
$ M_R = $ Race Weight Bonus Modifier (`Settings::f<PlayerRace>RaceMod`)

$ S_W = $ Weight per Stamina (simple method only) (`Settings::fWeightPerStamina`)\
$ S_B = $ Base Stamina Limit (`playerBaseStamina`)\
$ S_C = $ Current Stamina Limit (`playerStaminaAV`)\
$ S_\Delta = $ Stamina Rate (`Settings::fStaminaWeightRate`)\
$ S_P = $ Stamina Pivot Point (`Settings::uStaminaWeightPivot`)\
$ m_S = $ Gradient of stamina equation (`eqGrad`)\
$ S_M = $ Maximum player stamina value before default rate (+200 weight per stamina point) applies (`peakStam`)

$ L_W = $ Weight per Level (simple method only) (`Settings::fWeightPerLevel`)\
$ L_C = $ Current Player Level (`playerLevel`)\
$ L_\Delta = $ Level Rate (`Settings::fLevelWeightRate`)\
$ L_P = $ Level Pivot Point (`Settings::uLevelWeightPivot`)\
$ m_L = $ Gradient of level equation (`eqGrad`)\
$ L_M = $ Maximum player level before default rate (+200 weight per level) applies (`peakLvl`)

### Final Calculated Carry Weight
Used to determine the total carry weight to be applied to the player, based on the calculated bonuses $W_S,W_L$, the respective modifiers optionally used to modify their influence $M_S,M_L$, and the race-specific modifer applied to the resulting number $M_R$ .
$$ W_F = M_R(W_B + W_SM_S+W_LM_L) $$

### Stamina Weight Bonus (Simple)
Used to determine the bonus carry weight to be granted based on the player's current stamina limit $S_C$ (assuming that `Settings::bStaminaWeightSimple == true`).
$$ W_S = S_W(S_C-S_B) $$

### Stamina Weight Bonus (Complex)
Used to determine the bonus carry weight to be granted based on the player's current stamina limit $S_C$ (assuming that `Settings::bStaminaWeightSimple == false`).
$$ W_S = \frac{(S_C-S_B) - [S_\Delta(S_C-S_B)]}{S_\Delta-[S_\Delta(S_C-S_B)]+S_P}*\frac{W_B}{4} $$

### Stamina Weight Bonus Gradient
Used to determine the gradient of $W_S$ at the player's current stamina limit $S_C$ . If the gradient is too high then that indicates that the player's stamina is approaching the asymptote of the graph, beyond which the value of $W_S$ will become negative. The location of the asymptote is entirely dependant on the user's set values of `Settings::fStaminaWeightRate` and `Settings::uStaminaWeightPivot`, and so the plugin must be able to calculate the gradient as-needed. Comparing the result of this equation to the chosen "peak gradient" of 200 allows the code to intervene with a placeholder gradient (also 200) that will be applied to every stamina point the player has over the "peak stamina".
$$ \frac{dW_S}{dS_C} = m_S = \frac{W_B[-S_\Delta^2+S_\Delta-(S_\Delta S_P)+S_P]}{4[S_\Delta-S_\Delta(S_C-S_B)+S_P]^2} $$

### Player Stamina at $ m_S=200 $
In the event that the gradient $m_S$ of $W_S$ is found to be above 200, this equation is employed to determine the "peak stamina", i.e. the highest stamina value that is achieveable before the gradient surpasses 200. This helps to ensure that the placeholder stamina weight bonus gradient is applied *on top of* the player's current/prior weight bonus, which should hopefully retain a relative consistency in the growth rate of the weight bonus.
$$ S_M = \frac{\sqrt{W_B[-(S_\Delta-1)(S_\Delta+S_P)]}-S_\Delta20\sqrt{2}-S_P20\sqrt{2}}{S_\Delta20\sqrt{2}} + S_B $$

### Level Weight Bonus (Simple)
Used to determine the bonus carry weight to be granted based on the player's current level $L_C$ (assuming that `Settings::blevelWeightSimple == true`).
$$ W_L = L_WL_C $$

### Level Weight Bonus (Complex)
Used to determine the bonus carry weight to be granted based on the player's current level $L_C$ (assuming that `Settings::bLevelWeightSimple == false`).
$$ W_L = \frac{L_C - L_\Delta L_C}{L_\Delta-L_\Delta L_C+L_P}*\frac{W_B}{2} $$

### Level Weight Bonus Gradient
Same reasoning and explanation as for the equation for Stamina Weight Bonus Gradient $m_S$ .
$$ \frac{dW_L}{dL_C} = m_L = \frac{W_B(1-L_\Delta)(L_\Delta+L_P)}{2[L_\Delta-L_\Delta L_C+L_P]^2} $$

### Player Level at $ m_L=200 $
$$ L_M = -\frac{\sqrt{W_B(1-L_\Delta)(L_\Delta+L_P)}-20L_\Delta-20L_P}{20L_\Delta} $$