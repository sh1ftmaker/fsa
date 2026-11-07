#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/RailBase.h"
#include "game/act/act_advance_notice_se.h"
#include "game/act/act_all_enemies_defeated_check_switch.h"
#include "game/act/act_armos.h"
#include "game/act/act_arrow_marker.h"
#include "game/act/act_attack_hitbox_when_player_is_thrown.h"
#include "game/act/act_bandit_herding_npc.h"
#include "game/act/act_battle_royale_end_check.h"
#include "game/act/act_beamos.h"
#include "game/act/act_bg_rewrite_check_switch.h"
#include "game/act/act_bgm_change_npc.h"
#include "game/act/act_blade_trap.h"
#include "game/act/act_blindfold.h"
#include "game/act/act_blizzard_battle.h"
#include "game/act/act_block.h"
#include "game/act/act_block_for_2_3_players.h"
#include "game/act/act_bomb.h"
#include "game/act/act_bombchu.h"
#include "game/act/act_boss_display_test_npc.h"
#include "game/act/act_bti_npc.h"
#include "game/act/act_bumper.h"
#include "game/act/act_burning_block.h"
#include "game/act/act_buzz_blob.h"
#include "game/act/act_cactus.h"
#include "game/act/act_candlestick_switch.h"
#include "game/act/act_cannon.h"
#include "game/act/act_carrot.h"
#include "game/act/act_chain_switch.h"
#include "game/act/act_check_switch.h"
#include "game/act/act_cheep_cheep.h"
#include "game/act/act_chief_soldier.h"
#include "game/act/act_cloud.h"
#include "game/act/act_coconut.h"
#include "game/act/act_colour_block.h"
#include "game/act/act_crystal_ball.h"
#include "game/act/act_cucco.h"
#include "game/act/act_deadrock.h"
#include "game/act/act_debug_warp_settings.h"
#include "game/act/act_digging_game.h"
#include "game/act/act_door.h"
#include "game/act/act_dropped_item.h"
#include "game/act/act_dummy_conversation.h"
#include "game/act/act_ending_map_skipper.h"
#include "game/act/act_enemy_count_wipe_out_check.h"
#include "game/act/act_enemy_for_ending_control.h"
#include "game/act/act_enemy_generator_3.h"
#include "game/act/act_enemy_spawn_area.h"
#include "game/act/act_environment_control_2.h"
#include "game/act/act_escort_woman.h"
#include "game/act/act_explosion.h"
#include "game/act/act_extending_lift.h"
#include "game/act/act_eyeball_switch.h"
#include "game/act/act_eyegore.h"
#include "game/act/act_fairy.h"
#include "game/act/act_fairy_invincible.h"
#include "game/act/act_fairy_revival.h"
#include "game/act/act_falling_floor.h"
#include "game/act/act_fire_bar.h"
#include "game/act/act_fire_manager.h"
#include "game/act/act_fireball_generator.h"
#include "game/act/act_fish.h"
#include "game/act/act_floormaster.h"
#include "game/act/act_flying_tile.h"
#include "game/act/act_focus_npc.h"
#include "game/act/act_force_gem.h"
#include "game/act/act_force_gem_2.h"
#include "game/act/act_force_gem_buried.h"
#include "game/act/act_force_gem_fleeing.h"
#include "game/act/act_force_gem_rain.h"
#include "game/act/act_force_gem_shadow.h"
#include "game/act/act_force_gem_spawner.h"
#include "game/act/act_force_gem_winged.h"
#include "game/act/act_force_gem_wooden.h"
#include "game/act/act_four_person_pull_switch.h"
#include "game/act/act_four_person_pull_switch_pswb.h"
#include "game/act/act_four_person_pull_switch_pswk.h"
#include "game/act/act_four_switch_monitor.h"
#include "game/act/act_four_sword_demo.h"
#include "game/act/act_fuse_point.h"
#include "game/act/act_gambling_chest.h"
#include "game/act/act_gambling_shop.h"
#include "game/act/act_ganon.h"
#include "game/act/act_gba_appear.h"
#include "game/act/act_gba_window.h"
#include "game/act/act_gibdo.h"
#include "game/act/act_global_switch.h"
#include "game/act/act_grass.h"
#include "game/act/act_gravestone.h"
#include "game/act/act_great_fairy_spfr.h"
#include "game/act/act_hammer_bulge.h"
#include "game/act/act_hardhat_beetle.h"
#include "game/act/act_heart_container.h"
#include "game/act/act_horse_race_end_area.h"
#include "game/act/act_horse_race_man.h"
#include "game/act/act_hyrule_castle_barrier.h"
#include "game/act/act_ice_floor_check_npc.h"
#include "game/act/act_ice_melted_by_four_players.h"
#include "game/act/act_ice_wall.h"
#include "game/act/act_icicle.h"
#include "game/act/act_inextinguishable_flame.h"
#include "game/act/act_invisible_npc_wall.h"
#include "game/act/act_invisible_npc_wall_tomi.h"
#include "game/act/act_iron_ball_manager.h"
#include "game/act/act_item_pedestal.h"
#include "game/act/act_kaepora_gaebora.h"
#include "game/act/act_kaepora_gaebora_ending.h"
#include "game/act/act_link_ending_2.h"
#include "game/act/act_keese.h"
#include "game/act/act_knight_of_hyrule.h"
#include "game/act/act_leaf_effect_npc.h"
#include "game/act/act_leaf_effect_npc_generator.h"
#include "game/act/act_life_meter.h"
#include "game/act/act_lift.h"
#include "game/act/act_liftable_item.h"
#include "game/act/act_liftable_wood.h"
#include "game/act/act_link_ending.h"
#include "game/act/act_local_switch.h"
#include "game/act/act_log_monitor_npc.h"
#include "game/act/act_lotus.h"
#include "game/act/act_magic_circle.h"
#include "game/act/act_magic_circle_generator.h"
#include "game/act/act_magic_jar.h"
#include "game/act/act_merchant.h"
#include "game/act/act_message_check_npc.h"
#include "game/act/act_navi_maiden_crystal.h"
#include "game/act/act_npc_ride_switch.h"
#include "game/act/act_obj_lift.h"
#include "game/act/act_octorok.h"
#include "game/act/act_octorok_rock.h"
#include "game/act/act_off_switch.h"
#include "game/act/act_on_off_switch.h"
#include "game/act/act_opening_goal.h"
#include "game/act/act_panel_rewrite_npc.h"
#include "game/act/act_panel_rewrite_npc_2.h"
#include "game/act/act_particle_npc.h"
#include "game/act/act_pengator.h"
#include "game/act/act_phantom_ganon_2.h"
#include "game/act/act_play_specified_se.h"
#include "game/act/act_player_effect.h"
#include "game/act/act_player_start_position.h"
#include "game/act/act_poe.h"
#include "game/act/act_point_check_npc.h"
#include "game/act/act_popo.h"
#include "game/act/act_post_warp_pillar_handling.h"
#include "game/act/act_pot_2.h"
#include "game/act/act_pot_2_generator.h"
#include "game/act/act_pot_smoke.h"
#include "game/act/act_princess_zelda_demo.h"
#include "game/act/act_princess_zelda_ending.h"
#include "game/act/act_princess_zelda_motion_spawner.h"
#include "game/act/act_pull_switch.h"
#include "game/act/act_push_switch.h"
#include "game/act/act_push_switch_in_pyramid.h"
#include "game/act/act_quicksand.h"
#include "game/act/act_rank_display.h"
#include "game/act/act_rat.h"
#include "game/act/act_recovery_heart.h"
#include "game/act/act_red_blue_switch.h"
#include "game/act/act_rock.h"
#include "game/act/act_room_transfer_lift.h"
#include "game/act/act_rope.h"
#include "game/act/act_sand_crab.h"
#include "game/act/act_sandstorm.h"
#include "game/act/act_score.h"
#include "game/act/act_searchlight.h"
#include "game/act/act_seesaw_lift.h"
#include "game/act/act_shadow_link_2.h"
#include "game/act/act_shadow_link_bow.h"
#include "game/act/act_shadow_link_demo.h"
#include "game/act/act_shadow_link_generator.h"
#include "game/act/act_shadow_link_giant_bomb.h"
#include "game/act/act_shadow_link_jump.h"
#include "game/act/act_shop_item.h"
#include "game/act/act_shovel_old_timer.h"
#include "game/act/act_shutter.h"
#include "game/act/act_side_view_gate_display_npc.h"
#include "game/act/act_sign.h"
#include "game/act/act_slime_manager.h"
#include "game/act/act_slingshot_seed.h"
#include "game/act/act_small_key.h"
#include "game/act/act_somaria_block.h"
#include "game/act/act_spawner.h"
#include "game/act/act_special_bg_draw_object.h"
#include "game/act/act_special_conversation_npc.h"
#include "game/act/act_spike_floor_check.h"
#include "game/act/act_spotlight.h"
#include "game/act/act_spotlight_se.h"
#include "game/act/act_spreading_fire.h"
#include "game/act/act_stage_info_npc.h"
#include "game/act/act_stalfos.h"
#include "game/act/act_stone.h"
#include "game/act/act_stone_disguised_npc.h"
#include "game/act/act_stop_npc.h"
#include "game/act/act_strange_block.h"
#include "game/act/act_switch_or.h"
#include "game/act/act_switch_related_npc.h"
#include "game/act/act_sword_soldier.h"
#include "game/act/act_talking_npc.h"
#include "game/act/act_talking_npc_2.h"
#include "game/act/act_talking_sign.h"
#include "game/act/act_tektite.h"
#include "game/act/act_terrorpin.h"
#include "game/act/act_thief.h"
#include "game/act/act_thwomp.h"
#include "game/act/act_thwomp_launch_npc.h"
#include "game/act/act_thwomp_launch_npc_2.h"
#include "game/act/act_thwomp_switch.h"
#include "game/act/act_tingle_buster.h"
#include "game/act/act_tingle_first.h"
#include "game/act/act_tingle_time_up_manager.h"
#include "game/act/act_tingle_wallet.h"
#include "game/act/act_torch.h"
#include "game/act/act_tower_collapse_effect.h"
#include "game/act/act_treasure_chest.h"
#include "game/act/act_treasure_mark.h"
#include "game/act/act_trick_floor.h"
#include "game/act/act_trick_floor_2.h"
#include "game/act/act_tutorial_old_man.h"
#include "game/act/act_two_sword_motion.h"
#include "game/act/act_ura_colour.h"
#include "game/act/act_vaati_boss.h"
#include "game/act/act_vaati_buster.h"
#include "game/act/act_vaati_demo.h"
#include "game/act/act_vaati_time_up.h"
#include "game/act/act_view_mode.h"
#include "game/act/act_vulture.h"
#include "game/act/act_wait_switch.h"
#include "game/act/act_warp.h"
#include "game/act/act_warp_cannon.h"
#include "game/act/act_warp_entrance_wall.h"
#include "game/act/act_warp_pillar.h"
#include "game/act/act_wave_se.h"
#include "game/act/act_wind_environment.h"
#include "game/act/act_zelda_scroll_lock.h"
#include "game/act/act_zoom.h"
#include "game/act/act_zoom_out_npc.h"
#include "game/act/act_zora.h"
#include "game/act/act_zora_escort.h"
#include "game/act/act_zora_escort_goal.h"
#include "game/act/act_zora_lotus.h"
#include "game/act/tu_800F46C0.h"
#include "game/act/tu_800F7FCC.h"
#include "game/act/tu_800FF060.h"
#include "game/act/tu_8010089C.h"
#include "game/act/tu_80104C3C.h"
#include "game/act/tu_8011A0F8.h"
#include "game/act/tu_8011E96C.h"
#include "game/act/tu_801286C0.h"
#include "game/act/tu_801291A0.h"
#include "game/act/tu_8012DAC0.h"
#include "game/act/tu_8012EA00.h"
#include "game/act/tu_8012F8AC.h"
#include "game/act/tu_8012FBCC.h"
#include "game/act/tu_80130310.h"
#include "game/act/tu_80166BD0.h"
#include "game/act/tu_801688AC.h"
#include "game/act/tu_8017A44C.h"
#include "game/act/tu_80182914.h"
#include "game/act/tu_80183544.h"
#include "game/act/tu_80184D64.h"
#include "game/act/tu_801A8194.h"
#include "game/act/tu_801A908C.h"
#include "game/act/tu_801AE9C4.h"
#include "game/act/tu_801BD8C0.h"
#include "game/act/tu_801BEDBC.h"
#include "game/act/tu_801D7AFC.h"
#include "game/act/tu_801D8400.h"
#include "game/act/tu_801DB18C.h"
#include "game/act/tu_801DCC98.h"
#include "game/act/tu_801DE714.h"
#include "game/act/tu_801E6D48.h"
#include "game/act/tu_801E801C.h"
#include "game/act/tu_801F9484.h"
#include "game/act/tu_80209C14.h"
#include "game/act/tu_8020B354.h"
#include "game/act/tu_8020C0A8.h"
#include "game/act/tu_8020FE50.h"
#include "game/act/tu_80212E3C.h"
#include "game/act/tu_80213E00.h"
#include "game/act/tu_802201D8.h"
#include "game/act/tu_80221B44.h"
#include "game/act/tu_80222CC4.h"
#include "game/act/tu_8028DD78.h"
#include "game/act/tu_802957CC.h"
#include "game/act/tu_802B7C78.h"
#include "game/act/tu_802E2074.h"
#include "game/act/tu_802E6EC4.h"
#include "game/act/tu_802E9428.h"
#include "game/act/tu_802ED538.h"
#include "game/act/tu_802F3B74.h"
#include "game/act/tu_802F4AEC.h"
#include "game/act/tu_803276C4.h"
#include "game/act/tu_80333958.h"
#include "game/act/tu_8033F448.h"
#include "game/act/tu_80343A70.h"
#include "game/act/tu_803489A4.h"
#include "game/act/tu_8034A6B0.h"
#include "game/act/tu_8034AA40.h"
#include "game/act/tu_8034C950.h"
#include "game/act/tu_8034DFF0.h"
#include "game/act/tu_803536C4.h"
#include "game/act/tu_8036FB60.h"
#include "game/act/tu_80376FA0.h"
#include "game/cls_8049D740.h"
#include "game/cls_8049E350.h"
#include "game/cls_804A1630.h"
#include "game/cls_804A7BE8.h"
#include "game/cls_804A8588.h"
#include "game/cls_804A8638.h"
#include "game/tu_802EF76C.h"

// The view's per-screen byte the spawner stores in field_0x170 (fn_801FF674; weak, so
// it lives here, called out of line like the setters).
static inline s32 fn_801FF674() {
    return getGameView_802D6A9C()->fn_802D39FC();
}

// The spawner: finds the first free slot, builds the actor for `id` there, and fills in the
// fields every actor gets. The switch is every ActorId the game spawns; each case is one
// `new (mHeap, 0)` whose inline constructor lands after the spawner (in reverse order of use,
// like the setters below). DMCS, FGAM, ZSLE and ZSLG are empty cases (no actor, so -1).
s32 ActorManager_804A5800::spawnActor_801F9484(u32 id, s32 arg_0x4, const JGeometry::TVec3f* pos, u32 arg_0x90,
                                      s32 arg_0x16c, s32 arg_0x110) {
    s32 result = -1;
    for (s32 i = 0; i < ACTOR_MANAGER_MAX_ACTORS; i++) {
        if (mActors[i] != NULL) {
            continue;
        }
        switch (id) {
        case ACTOR_DMCS:
            break;
        case ACTOR_FGAM:
            break;
        case ACTOR_ZSLE:
            break;
        case ACTOR_ZSLG:
            break;
        case ACTOR_RUPY:
            mActors[i] = new (mHeap, 0) ForceGem_804AB6D8;
            break;
        case ACTOR_OKTA:
            mActors[i] = new (mHeap, 0) Octorok_8049F208;
            break;
        case ACTOR_AMOS:
            mActors[i] = new (mHeap, 0) Armos_8049F260;
            break;
        case ACTOR_POOH:
            mActors[i] = new (mHeap, 0) Poe_8049F330;
            break;
        case ACTOR_GRAS:
            mActors[i] = new (mHeap, 0) Grass_804A92D0;
            break;
        case ACTOR_GENE:
            mActors[i] = new (mHeap, 0) SpawnerGenerator_804A9328;
            break;
        case ACTOR_STON:
            mActors[i] = new (mHeap, 0) Stone_804A93D0;
            break;
        case ACTOR_BORD:
            mActors[i] = new (mHeap, 0) Sign_804A94D8;
            break;
        case ACTOR_WARP:
            mActors[i] = new (mHeap, 0) Warp_804A6DC8;
            break;
        case ACTOR_ROCK:
            mActors[i] = new (mHeap, 0) Rock_804A9480;
            break;
        case ACTOR_TUBO:
            mActors[i] = new (mHeap, 0) Pot_804A9428;
            break;
        case ACTOR_PEAC:
            mActors[i] = new (mHeap, 0) SandCrab_8049F3B0;
            break;
        case ACTOR_BLCK:
            mActors[i] = new (mHeap, 0) Block_804A7A10;
            break;
        case ACTOR_BZBR:
            mActors[i] = new (mHeap, 0) BuzzBlob_8049F444;
            break;
        case ACTOR_DDRK:
            mActors[i] = new (mHeap, 0) Deadrock_8049F498;
            break;
        case ACTOR_GOLD:
            mActors[i] = new (mHeap, 0) ForceGemSpawner_804AB660;
            break;
        case ACTOR_HART:
            mActors[i] = new (mHeap, 0) RecoveryHeart_8049F4F0;
            break;
        case ACTOR_OKTM:
            mActors[i] = new (mHeap, 0) OctorokRock_8049F5D8;
            break;
        case ACTOR_DGTL:
            mActors[i] = new (mHeap, 0) cls_8049F6C8;
            break;
        case ACTOR_BHRT:
            mActors[i] = new (mHeap, 0) HeartContainer_804A7E58;
            break;
        case ACTOR_GOAL:
            mActors[i] = new (mHeap, 0) cls_804A7EB0;
            break;
        case ACTOR_HINO:
            mActors[i] = new (mHeap, 0) Hinox_8049F9B4;
            break;
        case ACTOR_HBMB:
            mActors[i] = new (mHeap, 0) Bomb_8049F784;
            break;
        case ACTOR_SWTH:
            mActors[i] = new (mHeap, 0) Switch_804A7F38;
            break;
        case ACTOR_SLR1:
            mActors[i] = new (mHeap, 0) SwordSoldier_8049FCA0;
            break;
        case ACTOR_EXPL:
            mActors[i] = new (mHeap, 0) Explosion_804A7FB8;
            break;
        case ACTOR_TKTA:
            mActors[i] = new (mHeap, 0) Tektite_8049FD50;
            break;
        case 'SLR2':
            mActors[i] = new (mHeap, 0) cls_804A00C8;
            break;
        case ACTOR_IMUP:
            mActors[i] = new (mHeap, 0) LiftableItem_804A01D0;
            break;
        case ACTOR_BOYO:
            mActors[i] = new (mHeap, 0) Bumper_804A0260;
            break;
        case ACTOR_KETH:
            mActors[i] = new (mHeap, 0) Keese_804A02B8;
            break;
        case ACTOR_FLME:
            mActors[i] = new (mHeap, 0) Flame_804A0330;
            break;
        case ACTOR_JIJI:
            mActors[i] = new (mHeap, 0) cls_804AEED0;
            break;
        case ACTOR_TKYU:
            mActors[i] = new (mHeap, 0) IronBallManager_804ABA38;
            break;
        case ACTOR_MTBO:
            mActors[i] = new (mHeap, 0) MagicJar_804AF018;
            break;
        case ACTOR_RYRN:
            mActors[i] = new (mHeap, 0) ForceGemRain_804A8010;
            break;
        case ACTOR_SWT4:
            mActors[i] = new (mHeap, 0) FourSwitchMonitor_804A80A0;
            break;
        case ACTOR_FARY:
            mActors[i] = new (mHeap, 0) Fairy_804A03B8;
            break;
        case ACTOR_SHTR:
            mActors[i] = new (mHeap, 0) Shutter_804A8118;
            break;
        case ACTOR_BPOH:
            mActors[i] = new (mHeap, 0) cls_804A0440;
            break;
        case ACTOR_BALL:
            mActors[i] = new (mHeap, 0) IronBall_804AB968;
            break;
        case ACTOR_SWEV:
            mActors[i] = new (mHeap, 0) cls_804A8638;
            break;
        case ACTOR_THRN:
            mActors[i] = new (mHeap, 0) SpikeFloorCheck_804A8170;
            break;
        case ACTOR_TRAP:
            mActors[i] = new (mHeap, 0) BladeTrap_804A04D8;
            break;
        case ACTOR_ARRW:
            mActors[i] = new (mHeap, 0) ArrowPlayer_804A05E0;
            break;
        case ACTOR_PONT:
            mActors[i] = new (mHeap, 0) PointCheckNPC_804A81C8;
            break;
        case ACTOR_BFRY:
            mActors[i] = new (mHeap, 0) GreatFairy_804AF338;
            break;
        case ACTOR_MOZO:
            mActors[i] = new (mHeap, 0) MojamojaUnidentifiedEnemy_804A0668;
            break;
        case ACTOR_SKIL:
            mActors[i] = new (mHeap, 0) EnemyCountWipeOutCheck_804A8250;
            break;
        case ACTOR_Poon:
            mActors[i] = new (mHeap, 0) HardhatBeetle_804A06C0;
            break;
        case ACTOR_ITMS:
            mActors[i] = new (mHeap, 0) ItemPedestal_804A82A8;
            break;
        case ACTOR_PLAS:
            mActors[i] = new (mHeap, 0) PlayerStartPosition_804A8338;
            break;
        case ACTOR_KNBN:
            mActors[i] = new (mHeap, 0) TalkingSign_804AF428;
            break;
        case ACTOR_TFLR:
            mActors[i] = new (mHeap, 0) TrickFloor_804A83A0;
            break;
        case ACTOR_KMRI:
            mActors[i] = new (mHeap, 0) PotBreakSmoke_804A8418;
            break;
        case ACTOR_BOME:
            mActors[i] = new (mHeap, 0) cls_804A0738;
            break;
        case ACTOR_YAJI:
            mActors[i] = new (mHeap, 0) ArrowMarker_804A8470;
            break;
        case ACTOR_SPOT:
            mActors[i] = new (mHeap, 0) Spotlight_804A84C8;
            break;
        case ACTOR_SCRE:
            mActors[i] = new (mHeap, 0) Score_804AF4C0;
            break;
        case ACTOR_TRCH:
            mActors[i] = new (mHeap, 0) Torch_804A8520;
            break;
        case ACTOR_TALK:
            mActors[i] = new (mHeap, 0) TalkingNPC_804AF548;
            break;
        case ACTOR_SKUL:
            mActors[i] = new (mHeap, 0) RotatingSkull_804A8588;
            break;
        case ACTOR_SSTN:
            mActors[i] = new (mHeap, 0) NPCDisguisedAsAStone_804A8750;
            break;
        case ACTOR_SLSD:
            mActors[i] = new (mHeap, 0) SpotlightSE_804A85E0;
            break;
        case ACTOR_PEFF:
            mActors[i] = new (mHeap, 0) PlayerEffect_804A6E20;
            break;
        case 'SLR3':
            mActors[i] = new (mHeap, 0) cls_804A0810;
            break;
        case ACTOR_WOOD:
            mActors[i] = new (mHeap, 0) LiftableWood_804A87A8;
            break;
        case ACTOR_POPO:
            mActors[i] = new (mHeap, 0) Popo_804A0868;
            break;
        case ACTOR_SWOR:
            mActors[i] = new (mHeap, 0) SwitchOR_804A8800;
            break;
        case ACTOR_DETH:
            mActors[i] = new (mHeap, 0) ReaperShinigami_804A886C;
            break;
        case ACTOR_RY02:
            mActors[i] = new (mHeap, 0) ForceGem2_804AB850;
            break;
        case ACTOR_LIFT:
            mActors[i] = new (mHeap, 0) Lift_804A6F6C;
            break;
        case ACTOR_SWEL:
            mActors[i] = new (mHeap, 0) HammerBulge_804A88E8;
            break;
        case ACTOR_SBAL:
            mActors[i] = new (mHeap, 0) IronBallHoming_804AB9C0;
            break;
        case ACTOR_SLR4:
            mActors[i] = new (mHeap, 0) ChiefSoldier_804A0B90;
            break;
        case ACTOR_TLK2:
            mActors[i] = new (mHeap, 0) TalkingNpc2_804AFAD8;
            break;
        case ACTOR_DOSN:
            mActors[i] = new (mHeap, 0) ThwompSwitch_804A8940;
            break;
        case ACTOR_BGWC:
            mActors[i] = new (mHeap, 0) BgRewriteCheckSwitch_804A8998;
            break;
        case ACTOR_NIWA:
            mActors[i] = new (mHeap, 0) Cucco_804AB8C0;
            break;
        case ACTOR_FIRE:
            mActors[i] = new (mHeap, 0) FireRodFire_804A89F0;
            break;
        case ACTOR_BLZE:
            mActors[i] = new (mHeap, 0) SpreadingFire_804A8A50;
            break;
        case ACTOR_NZMI:
            mActors[i] = new (mHeap, 0) Rat_8049C400;
            break;
        case ACTOR_ROPE:
            mActors[i] = new (mHeap, 0) Rope_804A3A68;
            break;
        case ACTOR_OIWA:
            mActors[i] = new (mHeap, 0) Boulder_8049C4C0;
            break;
        case ACTOR_IWAG:
            mActors[i] = new (mHeap, 0) BoulderSpawner_8049C46C;
            break;
        case ACTOR_LPLK:
            mActors[i] = new (mHeap, 0) ForceLike_8049C578;
            break;
        case ACTOR_PAME:
            mActors[i] = new (mHeap, 0) Terrorpin_804A3B10;
            break;
        case ACTOR_ZNSW:
            mActors[i] = new (mHeap, 0) AllEnemiesDefeatedCheckSwitch_804A8AC8;
            break;
        case ACTOR_WTSW:
            mActors[i] = new (mHeap, 0) WaitSwitch_804A8B20;
            break;
        case ACTOR_BFIN:
            mActors[i] = new (mHeap, 0) BattleRoyaleEndCheck_804B0380;
            break;
        case ACTOR_BWRT:
            mActors[i] = new (mHeap, 0) cls_8049C718;
            break;
        case ACTOR_LWSW:
            mActors[i] = new (mHeap, 0) CandlestickSwitch_804A8B78;
            break;
        case ACTOR_WRTC:
            mActors[i] = new (mHeap, 0) StoneArrghusChildPuff_8049C678;
            break;
        case ACTOR_STAL:
            mActors[i] = new (mHeap, 0) Stalfos_804A0CD8;
            break;
        case ACTOR_TNGL:
            mActors[i] = new (mHeap, 0) cls_804B0478;
            break;
        case ACTOR_CKSW:
            mActors[i] = new (mHeap, 0) CheckSwitch_804A8BD0;
            break;
        case ACTOR_MIRA:
            mActors[i] = new (mHeap, 0) Gibdo_804A3BF8;
            break;
        case ACTOR_SWNP:
            mActors[i] = new (mHeap, 0) SwitchRelatedNpc_804A8C28;
            break;
        case ACTOR_WIZR:
            mActors[i] = new (mHeap, 0) Wizzrobe_804A0EDC;
            break;
        case ACTOR_LSTI:
            mActors[i] = new (mHeap, 0) DroppedItem_804A8C80;
            break;
        case ACTOR_KEY0:
            mActors[i] = new (mHeap, 0) SmallKey_804A8CD8;
            break;
        case ACTOR_DOOR:
            mActors[i] = new (mHeap, 0) Door_804A8D88;
            break;
        case ACTOR_ZORA:
            mActors[i] = new (mHeap, 0) ZoraEnemy_804A3D7C;
            break;
        case ACTOR_ZOSH:
            mActors[i] = new (mHeap, 0) cls_804A5810;
            break;
        case ACTOR_BOGF:
            mActors[i] = new (mHeap, 0) VaatiBoss_804A4958;
            break;
        case ACTOR_RAIL:
            mActors[i] = new (mHeap, 0) cls_8049C770;
            break;
        case ACTOR_FALL:
            mActors[i] = new (mHeap, 0) cls_804A3E44;
            break;
        case ACTOR_PNPC:
            mActors[i] = new (mHeap, 0) PanelRewriteNPC_804A8E40;
            break;
        case ACTOR_FOCS:
            mActors[i] = new (mHeap, 0) FocusNPC_804A8EA0;
            break;
        case ACTOR_BLDF:
            mActors[i] = new (mHeap, 0) Blindfold_804A8EF8;
            break;
        case ACTOR_HNRY:
            mActors[i] = new (mHeap, 0) WingedForceGem_804AB098;
            break;
        case ACTOR_RANK:
            mActors[i] = new (mHeap, 0) RankDisplay_804A8F74;
            break;
        case ACTOR_SHLT:
            mActors[i] = new (mHeap, 0) Searchlight_804A0FF8;
            break;
        case ACTOR_ESRY:
            mActors[i] = new (mHeap, 0) FleeingForceGem_804AB170;
            break;
        case ACTOR_TZOK:
            mActors[i] = new (mHeap, 0) Thief_804A5A30;
            break;
        case ACTOR_WDRY:
            mActors[i] = new (mHeap, 0) WoodenForceGem_804AB1F0;
            break;
        case ACTOR_LIF2:
            mActors[i] = new (mHeap, 0) ExtendingLift_804A78B0;
            break;
        case ACTOR_BRRY:
            mActors[i] = new (mHeap, 0) BuriedForceGem_804AB258;
            break;
        case ACTOR_FBMR:
            mActors[i] = new (mHeap, 0) FireBarManager_804A3F90;
            break;
        case ACTOR_FBOB:
            mActors[i] = new (mHeap, 0) cls_804A4038;
            break;
        case ACTOR_TINK:
            mActors[i] = new (mHeap, 0) cls_804A51E4;
            break;
        case ACTOR_TINB:
            mActors[i] = new (mHeap, 0) TingleBuster_804A5370;
            break;
        case ACTOR_OCFR:
            mActors[i] = new (mHeap, 0) FireManager_804AB2B0;
            break;
        case ACTOR_ONOF:
            mActors[i] = new (mHeap, 0) OnOffSwitch_804A8FC8;
            break;
        case ACTOR_RDBL:
            mActors[i] = new (mHeap, 0) RedBlueSwitch_804A9020;
            break;
        case ACTOR_CRTL:
            mActors[i] = new (mHeap, 0) CrystalBall_804AB308;
            break;
        case ACTOR_PCDM:
            mActors[i] = new (mHeap, 0) SlingshotSeed_804AB3A8;
            break;
        case 'BOIC':
            mActors[i] = new (mHeap, 0) cls_804A4C58;
            break;
        case ACTOR_BMTY:
            mActors[i] = new (mHeap, 0) Bombchu_804AB464;
            break;
        case ACTOR_RUSA:
            mActors[i] = new (mHeap, 0) cls_804A6270;
            break;
        case ACTOR_ARIJ:
            mActors[i] = new (mHeap, 0) cls_804A6340;
            break;
        case ACTOR_ARSH:
            mActors[i] = new (mHeap, 0) cls_804A58B4;
            break;
        case ACTOR_INFR:
            mActors[i] = new (mHeap, 0) InvincibilityFairy_804AB4C0;
            break;
        case ACTOR_CLOD:
            mActors[i] = new (mHeap, 0) Cloud_8049CBE0;
            break;
        case ACTOR_SWBM:
            mActors[i] = new (mHeap, 0) UnknownSWBM_804A1160;
            break;
        case ACTOR_PUKU:
            mActors[i] = new (mHeap, 0) CheepCheep_804A11B8;
            break;
        case ACTOR_OSIN:
            mActors[i] = new (mHeap, 0) cls_804AB528;
            break;
        case ACTOR_BMST:
            mActors[i] = new (mHeap, 0) Beamos_804A40E0;
            break;
        case ACTOR_LNBM:
            mActors[i] = new (mHeap, 0) cls_804A4188;
            break;
        case ACTOR_WLFT:
            mActors[i] = new (mHeap, 0) cls_8049CC40;
            break;
        case ACTOR_PLSW:
            mActors[i] = new (mHeap, 0) PullSwitch_804A7908;
            break;
        case 'FGAN':
            mActors[i] = new (mHeap, 0) cls_804A1318;
            break;
        case ACTOR_SLK2:
            mActors[i] = new (mHeap, 0) ShadowLink2_804A43E8;
            break;
        case ACTOR_WIND:
            mActors[i] = new (mHeap, 0) WindEnvironment_8049CC98;
            break;
        case ACTOR_FLYK:
            mActors[i] = new (mHeap, 0) FallingFloor_804A9078;
            break;
        case ACTOR_CNON:
            mActors[i] = new (mHeap, 0) Cannon_8049CCF0;
            break;
        case ACTOR_DEAL:
            mActors[i] = new (mHeap, 0) Merchant_8049CD48;
            break;
        case ACTOR_SITM:
            mActors[i] = new (mHeap, 0) ShopItem_8049CDC0;
            break;
        case ACTOR_CIRC:
            mActors[i] = new (mHeap, 0) MagicCircle_804A7960;
            break;
        case ACTOR_SBTN:
            mActors[i] = new (mHeap, 0) Cactus_804AB580;
            break;
        case ACTOR_GROK:
            mActors[i] = new (mHeap, 0) cls_804A1630;
            break;
        case ACTOR_DSIN:
            mActors[i] = new (mHeap, 0) Thwomp_804AB5D8;
            break;
        case ACTOR_RIVR:
            mActors[i] = new (mHeap, 0) cls_804A1718;
            break;
        case ACTOR_GDMN:
            mActors[i] = new (mHeap, 0) cls_804A1790;
            break;
        case ACTOR_FAGN:
            mActors[i] = new (mHeap, 0) Pengator_804A1888;
            break;
        case ACTOR_ZLDA:
            mActors[i] = new (mHeap, 0) cls_8049CEE0;
            break;
        case ACTOR_ICCL:
            mActors[i] = new (mHeap, 0) Icicle_8049CFC8;
            break;
        case ACTOR_SNBL:
            mActors[i] = new (mHeap, 0) cls_8049D020;
            break;
        case ACTOR_DBMS:
            mActors[i] = new (mHeap, 0) MessageCheckNPC_8049D0E8;
            break;
        case ACTOR_BTIN:
            mActors[i] = new (mHeap, 0) BTINPC_804A927C;
            break;
        case ACTOR_YASI:
            mActors[i] = new (mHeap, 0) Coconut_804A9968;
            break;
        case ACTOR_GRVE:
            mActors[i] = new (mHeap, 0) Gravestone_804A99C0;
            break;
        case ACTOR_TSNO:
            mActors[i] = new (mHeap, 0) TwoSwordMotion_804ABA90;
            break;
        case ACTOR_BOTS:
            mActors[i] = new (mHeap, 0) BossDisplayTestNpc_8049C7F0;
            break;
        case ACTOR_TNDR:
            mActors[i] = new (mHeap, 0) Vulture_804A4230;
            break;
        case ACTOR_TKRM:
            mActors[i] = new (mHeap, 0) TreasureMark_804ABAE8;
            break;
        case ACTOR_VWMD:
            mActors[i] = new (mHeap, 0) ViewMode_804A9A18;
            break;
        case ACTOR_URAC:
            mActors[i] = new (mHeap, 0) UraColour_804A9A70;
            break;
        case ACTOR_WNWN:
            mActors[i] = new (mHeap, 0) cls_804A18E0;
            break;
        case ACTOR_SLGE:
            mActors[i] = new (mHeap, 0) ShadowLinkGenerator_804A4310;
            break;
        case ACTOR_JPEN:
            mActors[i] = new (mHeap, 0) NPCLaunchedByThwomp_804AC010;
            break;
        case ACTOR_WPPR:
            mActors[i] = new (mHeap, 0) WarpPillar_804AC0A8;
            break;
        case ACTOR_FGN2:
            mActors[i] = new (mHeap, 0) PhantomGanon2_804A1A98;
            break;
        case ACTOR_RASN:
            mActors[i] = new (mHeap, 0) cls_804A7A68;
            break;
        case ACTOR_PRTC:
            mActors[i] = new (mHeap, 0) ParticleNpc_804A9AC8;
            break;
        case ACTOR_DMZL:
            mActors[i] = new (mHeap, 0) PrincessZeldaDemo_8049D208;
            break;
        case ACTOR_DMMD:
            mActors[i] = new (mHeap, 0) cls_8049D740;
            break;
        case 'DMCR':
            mActors[i] = new (mHeap, 0) cls_8049D83C;
            break;
        case ACTOR_DMGF:
            mActors[i] = new (mHeap, 0) cls_8049D940;
            break;
        case ACTOR_DMSW:
            mActors[i] = new (mHeap, 0) cls_8049DA0C;
            break;
        case ACTOR_BPH2:
            mActors[i] = new (mHeap, 0) cls_804A1E50;
            break;
        case 'DMCT':
            mActors[i] = new (mHeap, 0) cls_8049DAD8;
            break;
        case ACTOR_FLYO:
            mActors[i] = new (mHeap, 0) FlyingTile_804AC100;
            break;
        case ACTOR_WPPA:
            mActors[i] = new (mHeap, 0) PostWarpPillarHandling_804AC1F8;
            break;
        case ACTOR_RVFR:
            mActors[i] = new (mHeap, 0) RevivalFairy_804AC250;
            break;
        case ACTOR_DGT2:
            mActors[i] = new (mHeap, 0) cls_804A1FA0;
            break;
        case ACTOR_DMGL:
            mActors[i] = new (mHeap, 0) cls_8049DB30;
            break;
        case ACTOR_DMLK:
            mActors[i] = new (mHeap, 0) cls_8049DB90;
            break;
        case ACTOR_BGBL:
            mActors[i] = new (mHeap, 0) cls_804A4BB0;
            break;
        case ACTOR_DMGA:
            mActors[i] = new (mHeap, 0) cls_8049DD68;
            break;
        case ACTOR_STBL:
            mActors[i] = new (mHeap, 0) StrangeBlock_804AC2D8;
            break;
        case ACTOR_LOTS:
            mActors[i] = new (mHeap, 0) Lotus_804AC330;
            break;
        case ACTOR_ICHL:
            mActors[i] = new (mHeap, 0) cls_804A4EE8;
            break;
        case ACTOR_ICWL:
            mActors[i] = new (mHeap, 0) IceWall_8049DE18;
            break;
        case 'BLZD':
            mActors[i] = new (mHeap, 0) cls_804AC388;
            break;
        case ACTOR_BOIT:
            mActors[i] = new (mHeap, 0) cls_804A4F90;
            break;
        case ACTOR_LTZR:
            mActors[i] = new (mHeap, 0) cls_804AC3E0;
            break;
        case ACTOR_OFFS:
            mActors[i] = new (mHeap, 0) OFFSwitch_804A9B70;
            break;
        case ACTOR_GBAO:
            mActors[i] = new (mHeap, 0) GBAAppear_804B16E0;
            break;
        case ACTOR_BOIB:
            mActors[i] = new (mHeap, 0) cls_804A5038;
            break;
        case 'GEN2':
            mActors[i] = new (mHeap, 0) cls_804A9BC8;
            break;
        case ACTOR_BBSL:
            mActors[i] = new (mHeap, 0) ShadowLinkGiantBomb_804AC4D0;
            break;
        case ACTOR_BBMB:
            mActors[i] = new (mHeap, 0) GiantBomb_804AC588;
            break;
        case ACTOR_GNON:
            mActors[i] = new (mHeap, 0) Ganon_804A25F0;
            break;
        case 'ZLD2':
            mActors[i] = new (mHeap, 0) cls_804A2944;
            break;
        case ACTOR_HOUS:
            mActors[i] = new (mHeap, 0) cls_804A7AE0;
            break;
        case ACTOR_PSSW:
            mActors[i] = new (mHeap, 0) PushSwitch_804A7B38;
            break;
        case ACTOR_BEEE:
            mActors[i] = new (mHeap, 0) cls_804AC600;
            break;
        case ACTOR_GOL2:
            mActors[i] = new (mHeap, 0) cls_804A9C20;
            break;
        case ACTOR_NAVI:
            mActors[i] = new (mHeap, 0) cls_804A6708;
            break;
        case ACTOR_KEPO:
            mActors[i] = new (mHeap, 0) cls_804A6550;
            break;
        case ACTOR_LIMT:
            mActors[i] = new (mHeap, 0) LifeMeter_804B174C;
            break;
        case ACTOR_BRBL:
            mActors[i] = new (mHeap, 0) BurningBlockWoodenCrate_804A7B90;
            break;
        case ACTOR_IPOT:
            mActors[i] = new (mHeap, 0) cls_804A5418;
            break;
        case ACTOR_FSPO:
            mActors[i] = new (mHeap, 0) FusePoint_804A54C0;
            break;
        case ACTOR_FSOB:
            mActors[i] = new (mHeap, 0) cls_804A5568;
            break;
        case ACTOR_HAYA:
            mActors[i] = new (mHeap, 0) HammerShopMinigame_8049C858;
            break;
        case ACTOR_SENP:
            mActors[i] = new (mHeap, 0) AdvanceNoticeSE_804A9C78;
            break;
        case ACTOR_RNSW:
            mActors[i] = new (mHeap, 0) cls_8049DE90;
            break;
        case ACTOR_SEV2:
            mActors[i] = new (mHeap, 0) EnvironmentControl2_804A9CD0;
            break;
        case ACTOR_BARI:
            mActors[i] = new (mHeap, 0) HyruleCastleBarrier_8049DEE8;
            break;
        case ACTOR_BLZ2:
            mActors[i] = new (mHeap, 0) InextinguishableFlame_804A9D28;
            break;
        case ACTOR_FAZR:
            mActors[i] = new (mHeap, 0) ZoraEscort_8049DF78;
            break;
        case ACTOR_OPGL:
            mActors[i] = new (mHeap, 0) EscortWoman_8049E1D0;
            break;
        case ACTOR_OPGA:
            mActors[i] = new (mHeap, 0) cls_8049E350;
            break;
        case ACTOR_GFBS:
            mActors[i] = new (mHeap, 0) VaatiBuster_8049E3A8;
            break;
        case ACTOR_MGYA:
            mActors[i] = new (mHeap, 0) WhackAMoleShop_8049C8EC;
            break;
        case ACTOR_FZRA:
            mActors[i] = new (mHeap, 0) ZoraEscortGoal_8049E424;
            break;
        case ACTOR_BGMN:
            mActors[i] = new (mHeap, 0) BGMChangeNPC_804A9DA0;
            break;
        case ACTOR_ZOOM:
            mActors[i] = new (mHeap, 0) Zoom_804A9DF8;
            break;
        case ACTOR_ALCE:
            mActors[i] = new (mHeap, 0) AllEnemiesDefeated_804A2AB0;
            break;
        case ACTOR_FISH:
            mActors[i] = new (mHeap, 0) Fish_804AC658;
            break;
        case 'DMGE':
            mActors[i] = new (mHeap, 0) cls_804A9E50;
            break;
        case ACTOR_ANHO:
            mActors[i] = new (mHeap, 0) DiggingGameKeeper_8049C970;
            break;
        case ACTOR_SYKY:
            mActors[i] = new (mHeap, 0) ShootingGallery_804A2B18;
            break;
        case ACTOR_NNJN:
            mActors[i] = new (mHeap, 0) Carrot_804AC6B0;
            break;
        case ACTOR_SZSC:
            mActors[i] = new (mHeap, 0) ZeldaScrollLock_804AC708;
            break;
        case ACTOR_DMNR:
            mActors[i] = new (mHeap, 0) cls_8049E500;
            break;
        case ACTOR_SPBG:
            mActors[i] = new (mHeap, 0) SpecialBGDrawObject_804AC760;
            break;
        case ACTOR_FORM:
            mActors[i] = new (mHeap, 0) UnknownFORM_804A7BE8;
            break;
        case ACTOR_HRMN:
            mActors[i] = new (mHeap, 0) HorseRaceMan_804A5630;
            break;
        case ACTOR_WAVE:
            mActors[i] = new (mHeap, 0) WaveSe_804A9EA8;
            break;
        case ACTOR_TENN:
            mActors[i] = new (mHeap, 0) TennisShop_804A2BF4;
            break;
        case ACTOR_CRGN:
            mActors[i] = new (mHeap, 0) cls_8049E5B8;
            break;
        case ACTOR_SEN2:
            mActors[i] = new (mHeap, 0) PlaySpecifiedSe_804A9F00;
            break;
        case ACTOR_RNBW:
            mActors[i] = new (mHeap, 0) cls_8049E610;
            break;
        case ACTOR_FOBE:
            mActors[i] = new (mHeap, 0) BeeFormation_804AC7B8;
            break;
        case ACTOR_BEMN:
            mActors[i] = new (mHeap, 0) BeeManager_804AC810;
            break;
        case ACTOR_FOSL:
            mActors[i] = new (mHeap, 0) cls_804A5D60;
            break;
        case ACTOR_SLMN:
            mActors[i] = new (mHeap, 0) cls_804A5BD8;
            break;
        case ACTOR_FOBR:
            mActors[i] = new (mHeap, 0) cls_804A5E98;
            break;
        case ACTOR_BRMN:
            mActors[i] = new (mHeap, 0) cls_804A5C88;
            break;
        case ACTOR_SSLF:
            mActors[i] = new (mHeap, 0) SeesawLift_804AD348;
            break;
        case ACTOR_PNP2:
            mActors[i] = new (mHeap, 0) PanelRewriteNPC2_804A9F58;
            break;
        case ACTOR_ENBZ:
            mActors[i] = new (mHeap, 0) EnemySpawnArea_804A2CB0;
            break;
        case ACTOR_HTMK:
            mActors[i] = new (mHeap, 0) UnknownHTMK_804AC868;
            break;
        case ACTOR_MJGR:
            mActors[i] = new (mHeap, 0) MagicCircleGenerator_804A9FB0;
            break;
        case ACTOR_SAYA:
            mActors[i] = new (mHeap, 0) ZoraFishHerding_804A2E30;
            break;
        case ACTOR_NWOK:
            mActors[i] = new (mHeap, 0) cls_804AC8C0;
            break;
        case ACTOR_HBB2:
            mActors[i] = new (mHeap, 0) cls_804A2EC0;
            break;
        case ACTOR_ENPC:
            mActors[i] = new (mHeap, 0) EnemyForEndingControl_804AD48C;
            break;
        case ACTOR_EZLD:
            mActors[i] = new (mHeap, 0) PrincessZeldaEnding_804AD6F0;
            break;
        case ACTOR_EMIK:
            mActors[i] = new (mHeap, 0) MaidenEnding_804AD5B8;
            break;
        case ACTOR_GLBS:
            mActors[i] = new (mHeap, 0) GlobalSwitch_804AA060;
            break;
        case ACTOR_LCLS:
            mActors[i] = new (mHeap, 0) LocalSwitch_804AA008;
            break;
        case ACTOR_ENDM:
            mActors[i] = new (mHeap, 0) UnknownENDM_804AA0FC;
            break;
        case ACTOR_DDGO:
            mActors[i] = new (mHeap, 0) Dodongo_804A3334;
            break;
        case ACTOR_GOMA:
            mActors[i] = new (mHeap, 0) Gohma_804A307C;
            break;
        case ACTOR_TTAT:
            mActors[i] = new (mHeap, 0) cls_804A2FB0;
            break;
        case ACTOR_OKAM:
            mActors[i] = new (mHeap, 0) OkameSan_804A3448;
            break;
        case ACTOR_TKRA:
            mActors[i] = new (mHeap, 0) TreasureChest_804AA178;
            break;
        case ACTOR_DTLK:
            mActors[i] = new (mHeap, 0) DummyConversation_804B1A48;
            break;
        case ACTOR_ELNK:
            mActors[i] = new (mHeap, 0) LinkEnding_804AD858;
            break;
        case ACTOR_SMRI:
            mActors[i] = new (mHeap, 0) CaneOfSomariaBlock_804AA9F0;
            break;
        case ACTOR_ZASL:
            mActors[i] = new (mHeap, 0) cls_804A5F40;
            break;
        case ACTOR_DMTW:
            mActors[i] = new (mHeap, 0) cls_8049E770;
            break;
        case ACTOR_EOWL:
            mActors[i] = new (mHeap, 0) KaeporaGaeboraEnding_804ADAC8;
            break;
        case ACTOR_RCK2:
            mActors[i] = new (mHeap, 0) cls_804AC950;
            break;
        case ACTOR_EETC:
            mActors[i] = new (mHeap, 0) cls_804ADF28;
            break;
        case ACTOR_TWTG:
            mActors[i] = new (mHeap, 0) cls_804AAD60;
            break;
        case ACTOR_GMBL:
            mActors[i] = new (mHeap, 0) GamblingShop_804AAA70;
            break;
        case ACTOR_GMTB:
            mActors[i] = new (mHeap, 0) GamblingShopChest_804AAAE8;
            break;
        case ACTOR_FLOR:
            mActors[i] = new (mHeap, 0) Floormaster_804A6004;
            break;
        case ACTOR_YNSW:
            mActors[i] = new (mHeap, 0) cls_804AC9A8;
            break;
        case ACTOR_EGOL:
            mActors[i] = new (mHeap, 0) Eyegore_804A3548;
            break;
        case ACTOR_GEN3:
            mActors[i] = new (mHeap, 0) EnemyGenerator3_804AA1D0;
            break;
        case ACTOR_OBLF:
            mActors[i] = new (mHeap, 0) OBJLift_804ACA00;
            break;
        case ACTOR_ZLD3:
            mActors[i] = new (mHeap, 0) cls_804A35AC;
            break;
        case 'ZLD4':
            mActors[i] = new (mHeap, 0) cls_804A3698;
            break;
        case ACTOR_ZDCO:
            mActors[i] = new (mHeap, 0) PrincessZeldaMotionObjectSpawner_804A375C;
            break;
        case ACTOR_IBBM:
            mActors[i] = new (mHeap, 0) cls_804A6100;
            break;
        case ACTOR_ZOMO:
            mActors[i] = new (mHeap, 0) ZoomOutNPC_804AA228;
            break;
        case ACTOR_DNAV:
            mActors[i] = new (mHeap, 0) Navi54MaidenCrystalCutscene_804ADFE8;
            break;
        case ACTOR_DCR2:
            mActors[i] = new (mHeap, 0) cls_804AE088;
            break;
        case ACTOR_SVTG:
            mActors[i] = new (mHeap, 0) ShovelOldTimer_804AAB40;
            break;
        case ACTOR_BTBZ:
            mActors[i] = new (mHeap, 0) cls_8049E7F0;
            break;
        case ACTOR_BCK2:
            mActors[i] = new (mHeap, 0) ColourBlock1Player_804AE620;
            break;
        case ACTOR_SDSM:
            mActors[i] = new (mHeap, 0) Sandstorm_804AA280;
            break;
        case ACTOR_SLEF:
            mActors[i] = new (mHeap, 0) cls_804A6400;
            break;
        case ACTOR_MEDM:
            mActors[i] = new (mHeap, 0) EyeballSwitch_804AE678;
            break;
        case ACTOR_TOMI:
            mActors[i] = new (mHeap, 0) InvisibleNPCWall_804AE6D0;
            break;
        case ACTOR_STPN:
            mActors[i] = new (mHeap, 0) StopNPC_804AA2D8;
            break;
        case ACTOR_GNEF:
            mActors[i] = new (mHeap, 0) cls_804AA350;
            break;
        case ACTOR_PSW2:
            mActors[i] = new (mHeap, 0) FourPersonPullSwitch_804AE230;
            break;
        case ACTOR_RBPN:
            mActors[i] = new (mHeap, 0) cls_8049E848;
            break;
        case ACTOR_JWLK:
            mActors[i] = new (mHeap, 0) KnightOfHyrule_804AABA8;
            break;
        case ACTOR_TLWP:
            mActors[i] = new (mHeap, 0) cls_804A38D4;
            break;
        case ACTOR_CHSW:
            mActors[i] = new (mHeap, 0) ChainSwitch_804AA3E8;
            break;
        case ACTOR_TOOI:
            mActors[i] = new (mHeap, 0) BanditHerdingNPC_804ACB40;
            break;
        case ACTOR_DWAP:
            mActors[i] = new (mHeap, 0) DebugWarpSettings_8049C9E8;
            break;
        case ACTOR_TWEV:
            mActors[i] = new (mHeap, 0) TowerCollapseEffect_8049CA40;
            break;
        case ACTOR_SNWL:
            mActors[i] = new (mHeap, 0) cls_8049E8A0;
            break;
        case ACTOR_FDWP:
            mActors[i] = new (mHeap, 0) cls_8049E8F8;
            break;
        case ACTOR_SAFU:
            mActors[i] = new (mHeap, 0) TinglesWallet_804A3948;
            break;
        case ACTOR_TMI2:
            mActors[i] = new (mHeap, 0) BlockFor2Or3Players_804AE728;
            break;
        case ACTOR_SVGT:
            mActors[i] = new (mHeap, 0) SideViewGateDisplayNpc_804ACB98;
            break;
        case ACTOR_AGBW:
            mActors[i] = new (mHeap, 0) GBAWindowControl_804AA440;
            break;
        case ACTOR_THCL:
            mActors[i] = new (mHeap, 0) AttackHitboxWhenPlayerIsThrown_804A7C40;
            break;
        case ACTOR_LFGN:
            mActors[i] = new (mHeap, 0) LeafEffectNPCGenerator_804AE288;
            break;
        case ACTOR_LEAF:
            mActors[i] = new (mHeap, 0) LeafEffectNPC_804AE2E0;
            break;
        case ACTOR_TUGF:
            mActors[i] = new (mHeap, 0) VaatiTimeUp_804ACCC0;
            break;
        case 'TUTL':
            mActors[i] = new (mHeap, 0) cls_804ACC68;
            break;
        case ACTOR_TUTM:
            mActors[i] = new (mHeap, 0) TingleTimeUpManager_804ACBF0;
            break;
        case ACTOR_EMSK:
            mActors[i] = new (mHeap, 0) EndingMapSkipper_804AA498;
            break;
        case ACTOR_IFCK:
            mActors[i] = new (mHeap, 0) cls_8049E950;
            break;
        case ACTOR_ICOB:
            mActors[i] = new (mHeap, 0) cls_8049E9A8;
            break;
        case ACTOR_OGST:
            mActors[i] = new (mHeap, 0) cls_8049EA00;
            break;
        case ACTOR_RSNP:
            mActors[i] = new (mHeap, 0) ForceGemShadowNPC_804ACD18;
            break;
        case ACTOR_LTEV:
            mActors[i] = new (mHeap, 0) cls_804AA4F0;
            break;
        case ACTOR_MRMT:
            mActors[i] = new (mHeap, 0) cls_8049EA88;
            break;
        case ACTOR_FBGE:
            mActors[i] = new (mHeap, 0) FireballGenerator_804A6660;
            break;
        case ACTOR_FBAL:
            mActors[i] = new (mHeap, 0) cls_804A6828;
            break;
        case ACTOR_PTMI:
            mActors[i] = new (mHeap, 0) InvisibleNPCWallForLink_804AE780;
            break;
        case ACTOR_ELK2:
            mActors[i] = new (mHeap, 0) Link2Ending_804AE338;
            break;
        case 'FGAL':
            mActors[i] = new (mHeap, 0) cls_804AE4B0;
            break;
        case 'RCK3':
            mActors[i] = new (mHeap, 0) cls_804AE7D8;
            break;
        case ACTOR_FTGL:
            mActors[i] = new (mHeap, 0) cls_804AADD8;
            break;
        case ACTOR_SWDL:
            mActors[i] = new (mHeap, 0) PlayerSwordLevelCheckNPC_804AA590;
            break;
        case ACTOR_WPCN:
            mActors[i] = new (mHeap, 0) WarpCannon_8049EAE0;
            break;
        case ACTOR_MRTA:
            mActors[i] = new (mHeap, 0) cls_804ACD70;
            break;
        case ACTOR_TJIJ:
            mActors[i] = new (mHeap, 0) TutorialOldMan_804AE508;
            break;
        case ACTOR_SNPC:
            mActors[i] = new (mHeap, 0) SpecialConversationNPC_804AAFB8;
            break;
        case ACTOR_GLWP:
            mActors[i] = new (mHeap, 0) cls_804A39DC;
            break;
        case ACTOR_SPFR:
            mActors[i] = new (mHeap, 0) cls_8049EB8C;
            break;
        case ACTOR_PSWB:
            mActors[i] = new (mHeap, 0) FourPersonPullSwitchBTIVersion_804AE574;
            break;
        case ACTOR_SIBT:
            mActors[i] = new (mHeap, 0) TempleBTIDisplay_8049CB04;
            break;
        case ACTOR_PSWK:
            mActors[i] = new (mHeap, 0) FourPersonPullSwitchChainVersion_804AE5C8;
            break;
        case ACTOR_LOSW:
            mActors[i] = new (mHeap, 0) cls_8049CB58;
            break;
        case ACTOR_TFL2:
            mActors[i] = new (mHeap, 0) TrickFloor2_804AA628;
            break;
        case ACTOR_ENDR:
            mActors[i] = new (mHeap, 0) HorseRaceEndArea_804AE830;
            break;
        case ACTOR_JPE2:
            mActors[i] = new (mHeap, 0) NPCLaunchedByThwomp2_804AD1C8;
            break;
        case ACTOR_DMYK:
            mActors[i] = new (mHeap, 0) UnknownDMYK_804AA6A0;
            break;
        case ACTOR_ICEB:
            mActors[i] = new (mHeap, 0) IceMeltedByFourPlayers_804AE888;
            break;
        case ACTOR_TBGN:
            mActors[i] = new (mHeap, 0) Pot2Generator_804AA6F8;
            break;
        case ACTOR_TBO2:
            mActors[i] = new (mHeap, 0) Pot2_804AA750;
            break;
        case ACTOR_CRNV:
            mActors[i] = new (mHeap, 0) cls_8049ED30;
            break;
        case ACTOR_ZSLJ:
            mActors[i] = new (mHeap, 0) cls_804A68D0;
            break;
        case 'ZSLH':
            mActors[i] = new (mHeap, 0) cls_804A6AA8;
            break;
        case ACTOR_ZSLA:
            mActors[i] = new (mHeap, 0) cls_804A6B70;
            break;
        case 'ZSLF':
            mActors[i] = new (mHeap, 0) cls_804A6C38;
            break;
        case 'ZSLD':
            mActors[i] = new (mHeap, 0) cls_804A6D00;
            break;
        case ACTOR_PSS2:
            mActors[i] = new (mHeap, 0) PushSwitchInPyramid_804AE8F4;
            break;
        case ACTOR_WPWL:
            mActors[i] = new (mHeap, 0) WarpEntranceWall_804AE948;
            break;
        case ACTOR_RINS:
            mActors[i] = new (mHeap, 0) NPCRideSwitch_804AD220;
            break;
        case ACTOR_STGN:
            mActors[i] = new (mHeap, 0) StageInfoNpc_804AA7A8;
            break;
        case ACTOR_MRTC:
            mActors[i] = new (mHeap, 0) LogMonitorNpc_804AD278;
            break;
        case ACTOR_GSCK:
            mActors[i] = new (mHeap, 0) UnknownGSCK_804AA800;
            break;
        }
        if (mActors[i] == NULL) {
            return -1;
        }
        mSounds[i]->setField_0x31();
        mSounds[i]->fn_80457840(mActors[i]->getPos());
        mActors[i]->setSerial(mSpawnCount);
        mActors[i]->setSlot(i);
        mActors[i]->setPos(*pos);
        mActors[i]->setSpawnPos(*pos);
        mActors[i]->setActorId(id);
        mActors[i]->setField_0x4(arg_0x4);
        mActors[i]->setField_0x8(arg_0x4);
        mActors[i]->setField_0x90(arg_0x90);
        mActors[i]->setLife(fn_801F620C(id, ACTOR_PARAM_LIFE));
        mActors[i]->setField_0x114(fn_801F620C(id, ACTOR_PARAM_ATCP));
        mActors[i]->setField_0x118(fn_801F620C(id, ACTOR_PARAM_ATCN));
        mActors[i]->setField_0x1A8(fn_801F620C(id, ACTOR_PARAM_DSTR) != 0);
        mActors[i]->setField_0x18C(fn_801F620C(id, ACTOR_PARAM_DASU));
        mActors[i]->setField_0xC0(fn_801F620C(id, ACTOR_PARAM_EFSZ));
        mActors[i]->setField_0x190(fn_801F620C(id, ACTOR_PARAM_VOIC));
        mActors[i]->setField_0x16C(arg_0x16c);
        mActors[i]->setField_0x170(fn_801FF674());
        mActors[i]->setField_0x110(arg_0x110);
        mLastSlot = i;
        result = mSpawnCount;
        mSpawnCount = result + 1;
        if (mActorEnd < i + 1) {
            mActorEnd = i + 1;
        }
        mActors[i]->init();
        if (mActors[i]->getBox2()->i.x == 0.0f && mActors[i]->getBox2()->i.y == 0.0f && mActors[i]->getBox2()->f.x == 0.0f &&
            mActors[i]->getBox2()->f.y == 0.0f) {
            mActors[i]->setBox2(*mActors[i]->getBox());
        }
        break;
    }
    return result;
}
