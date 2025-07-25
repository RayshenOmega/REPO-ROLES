//This code has been published on the 20th of July in 2025 and is owned by KingRobin72.
//Modifications of the code are allowed as long as the modifications are not malicios.
//This mod is made in Visual Studio 2022 using the linknoid repo extension for it.

//------------------------------------//

using BepInEx;
using HarmonyLib;
using UnityEngine;
using BepInEx.Logging;
using R.E.P.O.Roles;
using UnityEngine.SceneManagement;
using R.E.P.O.Roles.patches;
using BepInEx.Configuration;
using MenuLib;
using MenuLib.MonoBehaviors;
using Photon.Pun;
using System.Collections;
using Unity.VisualScripting;
using System.IO;
using System.Reflection;

namespace Bobi
{
    [BepInPlugin(pluginGuid, pluginName, pluginVersion)]
    public class RepoRoles : BaseUnityPlugin
    {
        public const string pluginGuid = "reporoles.mod";
        public const string pluginName = "RepoRoles";
        public const string pluginVersion = "2.0.6";

        public readonly Harmony harmony = new Harmony(pluginGuid);
        public static RepoRoles Instance;
        internal ManualLogSource mls;


        public static Font newFont;
        public static guiManager GUIinstance;

        public static ConfigEntry<KeyCode> toggleKey;
        public static ConfigEntry<KeyCode> showSpellsKey;
        public static ConfigEntry<KeyCode> healKey;
        public static ConfigEntry<KeyCode> speedKey;
        public static ConfigEntry<KeyCode> overchargeKey;
        public static ConfigEntry<KeyCode> jumpKey;
        public static ConfigEntry<KeyCode> staminaKey;
        public static ConfigEntry<KeyCode> scoutKey;


        public static ConfigEntry<string> customRoleNameRunner;
        public static ConfigEntry<string> customRoleNameTank;
        public static ConfigEntry<string> customRoleNameGambler;
        public static ConfigEntry<string> customRoleNameStrongman;
        public static ConfigEntry<string> customRoleNameRL;
        public static ConfigEntry<string> customRoleNameAthletic;
        public static ConfigEntry<string> customRoleNameMage;
        public static ConfigEntry<string> customRoleNameReaper;
        public static ConfigEntry<string> customRoleNameScout;

        public static ConfigEntry<string> customRoleDecRunner;
        public static ConfigEntry<string> customRoleDecTank;
        public static ConfigEntry<string> customRoleDecGambler;
        public static ConfigEntry<string> customRoleDecStrongman;
        public static ConfigEntry<string> customRoleDecRL;
        public static ConfigEntry<string> customRoleDecAthletic;
        public static ConfigEntry<string> customRoleDecMage;
        public static ConfigEntry<string> customRoleDecReaper;
        public static ConfigEntry<string> customRoleDecScout;


        public static ConfigEntry<string> savedRole;
        public static ConfigEntry<bool> assignRoleAfterRevive;
        public static ConfigEntry<bool> mageTopManaConf;

        public static ConfigEntry<bool> showGUIAtStart;
        public ConfigDefinition showGUIAtStartDef = new ConfigDefinition("GUI", "Show GUI when being assigned a role");

        public ConfigDefinition mageTopManaDef = new ConfigDefinition("Role","Place where mana is shown");

        public ConfigDefinition def = new ConfigDefinition("GUI", "GUI Toggle Key");
        public ConfigDefinition scoutButtonDef = new ConfigDefinition("GUI", "Activate Scout Ability");


        public ConfigDefinition customRoleNameRunnerDef = new ConfigDefinition("Role Names", "Runner Name");
        public ConfigDefinition customRoleNameTankDef = new ConfigDefinition("Role Names", "Tank Name");
        public ConfigDefinition customRoleNameGamblerDef = new ConfigDefinition("Role Names", "Gambler Name");
        public ConfigDefinition customRoleNameStrongmanDef = new ConfigDefinition("Role Names", "Strongman Name");
        public ConfigDefinition customRoleNameRLDef = new ConfigDefinition("Role Names", "Ranged Looter Name");
        public ConfigDefinition customRoleNameAthleticDef = new ConfigDefinition("Role Names", "Athletic Name");
        public ConfigDefinition customRoleNameMageDef = new ConfigDefinition("Role Names", "Mage Name");
        public ConfigDefinition customRoleNameReaperDef = new ConfigDefinition("Role Names", "Reaper Name");
        public ConfigDefinition customRoleNameScoutDef = new ConfigDefinition("Role Names", "Scout Name");

        public ConfigDefinition customRoleDesRunnerDef = new ConfigDefinition("Role Descriptions", "Runner Description");
        public ConfigDefinition customRoleDesTankDef = new ConfigDefinition("Role Descriptions", "Tank Description");
        public ConfigDefinition customRoleDesGamblerDef = new ConfigDefinition("Role Descriptions", "Gambler Description");
        public ConfigDefinition customRoleDesStrongmanDef = new ConfigDefinition("Role Descriptions", "Strongman Description");
        public ConfigDefinition customRoleDesRLDef = new ConfigDefinition("Role Descriptions", "Ranged Looter Description");
        public ConfigDefinition customRoleDesAthleticDef = new ConfigDefinition("Role Descriptions", "Athletic Description");
        public ConfigDefinition customRoleDesMageDef = new ConfigDefinition("Role Descriptions", "Mage Description");
        public ConfigDefinition customRoleDesReaperDef = new ConfigDefinition("Role Descriptions", "Reaper Description");
        public ConfigDefinition customRoleDesScoutDef = new ConfigDefinition("Role Descriptions", "Scout Description");

        public ConfigDefinition showSpellsDef = new ConfigDefinition("Mage", "Show Spells");
        public ConfigDefinition healDef = new ConfigDefinition("Mage", "Healing Spell");
        public ConfigDefinition speedDef = new ConfigDefinition("Mage", "Speed Spell");
        public ConfigDefinition overchargeDef = new ConfigDefinition("Mage", "Overcharge Spell");
        public ConfigDefinition jumpDef = new ConfigDefinition("Mage", "Jump Boost Spell");
        public ConfigDefinition staminaDef = new ConfigDefinition("Mage", "Stamina Refill Spell");
        public ConfigDefinition selectedRoleDef = new ConfigDefinition("Role", "Your selected role");
        public ConfigDefinition afterDeathRoleDef = new ConfigDefinition("Role", "Reaasign roles after being revived");


        public static ConfigEntry<bool> enableRunner;
        public static ConfigEntry<bool> enableTank;
        public static ConfigEntry<bool> enableGambler;
        public static ConfigEntry<bool> enableStrongman;
        public static ConfigEntry<bool> enableRL;
        public static ConfigEntry<bool> enableAthletic;
        public static ConfigEntry<bool> enableMage;
        public static ConfigEntry<bool> enableReaper;
        public static ConfigEntry<bool> enableScout;

        public ConfigDefinition enableRunnerDef = new ConfigDefinition("Role", "Enable Runner");
        public ConfigDefinition enableTankDef = new ConfigDefinition("Role", "Enable Tank");
        public ConfigDefinition enableGamblerDef = new ConfigDefinition("Role", "Enable Gambler");
        public ConfigDefinition enableStrongmanDef = new ConfigDefinition("Role", "Enable Strongman");
        public ConfigDefinition enableRLDef = new ConfigDefinition("Role", "Enable Ranged Looter");
        public ConfigDefinition enableAthleticDef = new ConfigDefinition("Role", "Enable Athletic");
        public ConfigDefinition enableMageDef = new ConfigDefinition("Role", "Enable Mage");
        public ConfigDefinition enableReaperDef = new ConfigDefinition("Role", "Enable Reaper");
        public ConfigDefinition enableScoutDef = new ConfigDefinition("Role", "Enable Scout");


        REPOSlider slider;
        REPOPopupPage configPage;
        REPOButton buttonOpen;
        REPOButton buttonOpenLobby;
        REPOButton buttonClose;
        REPOToggle afterDeathRoleToggle;
        REPOToggle mageTopMana;


        public static bool afterDeathNewRole = false;
        public static bool mageTopManaBool;
        public static string chosenRoleName = "Random";

        public void sliderConf(string a)
        {
            savedRole.Value = a;
            chosenRoleName = a;
        }
        public void aDRC(bool a)
        {
            assignRoleAfterRevive.Value = a;
            afterDeathNewRole = a;
        }
        public void mageManaFunc(bool a)
        {
            mageTopManaConf.Value = a;
            mageTopManaBool = a;
        }


        public static string getPath()
        {
            string path = Path.Combine(Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location), "REPORoles_assets2");
            return path;
        }

        private void Awake()
        {
            mls = BepInEx.Logging.Logger.CreateLogSource(pluginGuid);
            savedRole = Config.Bind(selectedRoleDef, "Random");
            assignRoleAfterRevive = Config.Bind(afterDeathRoleDef, false);
            mageTopManaConf = Config.Bind(mageTopManaDef, true);
            mageTopManaBool = mageTopManaConf.Value;

            showGUIAtStart = Config.Bind(showGUIAtStartDef, true);


            enableRunner = Config.Bind(enableRunnerDef, true);
            enableTank = Config.Bind(enableTankDef, true);
            enableGambler = Config.Bind(enableGamblerDef, true);
            enableStrongman = Config.Bind(enableStrongmanDef, true);
            enableRL = Config.Bind(enableRLDef, true);
            enableAthletic = Config.Bind(enableAthleticDef, true);
            enableMage = Config.Bind(enableMageDef, true);
            enableReaper = Config.Bind(enableReaperDef, true);
            enableScout = Config.Bind(enableScoutDef, true);


            MenuAPI.AddElementToSettingsMenu(parent =>
            {
                if (configPage == null)
                {
                    configPage = MenuAPI.CreateREPOPopupPage("REPO Roles", REPOPopupPage.PresetSide.Left, shouldCachePage: true, pageDimmerVisibility: true, spacing: 1.5f);
                }

                buttonOpen = MenuAPI.CreateREPOButton("REPO Roles Config", () => configPage.OpenPage(false), parent, localPosition: new Vector2(500, 10));


                configPage.AddElementToScrollView(scrollView =>
                {
                    if (slider == null)
                    {
                        slider = MenuAPI.CreateREPOSlider("REPO Roles", "Choose your role", (string s) => sliderConf(s), scrollView, stringOptions: ["Random", "Runner", "Tank", "Gambler", "Strongman", "Ranged Looter", "Athletic", "Mage", "Reaper", "Scout"], defaultOption: savedRole.Value, localPosition: new Vector2(0, 0), "", "", REPOSlider.BarBehavior.UpdateWithValue);
                    }
                    return slider.rectTransform;
                });
                configPage.AddElementToScrollView(scrollView =>
                {
                    if (afterDeathRoleToggle == null)
                    {
                        afterDeathRoleToggle = MenuAPI.CreateREPOToggle("Reassign role on respawn", b => aDRC(b), scrollView, new Vector2(0, 0), "ON", "OFF", defaultValue: assignRoleAfterRevive.Value);
                    }
                    return afterDeathRoleToggle.rectTransform;
                });
                configPage.AddElementToScrollView(scrollView =>
                {
                    if (mageTopMana == null)
                    {
                        mageTopMana = MenuAPI.CreateREPOToggle("Mage Mana Position", b => mageManaFunc(b), scrollView, new Vector2(0, 0), "TOP", "LEFT", defaultValue: mageTopManaConf.Value);
                    }
                    return mageTopMana.rectTransform;
                });


                configPage.AddElement(parent =>
                {
                    if (buttonClose == null)
                    {
                        buttonClose = MenuAPI.CreateREPOButton("Back", () => configPage.ClosePage(false), parent, localPosition: Vector2.zero);
                    }
                });
            });

            MenuAPI.AddElementToLobbyMenu(parent =>
            {
                buttonOpenLobby = MenuAPI.CreateREPOButton("REPO Roles Config", () => configPage.OpenPage(false), parent, localPosition: new Vector2(500, 500));
            });



            if (Instance == null)
            {
                Instance = this;
            }

            harmony.PatchAll(typeof(RepoRoles));
            harmony.PatchAll(typeof(Sender));


            mls.LogInfo("Init");

            SceneManager.sceneLoaded += (arg0, mode) =>
            {
                if (GUIinstance)
                    return;

                var obj = new GameObject();
                GUIinstance = obj.AddComponent<guiManager>();
                GUIinstance.text = "";
                GUIinstance.color = Color.cyan;
                DontDestroyOnLoad(obj);
            };

            toggleKey = Config.Bind(def, KeyCode.R);
            showSpellsKey = Config.Bind(showSpellsDef, KeyCode.M);
            healKey = Config.Bind(healDef, KeyCode.H);
            speedKey = Config.Bind(speedDef, KeyCode.J);
            overchargeKey = Config.Bind(overchargeDef, KeyCode.O);
            jumpKey = Config.Bind(jumpDef, KeyCode.K);
            staminaKey = Config.Bind(staminaDef, KeyCode.N);
            customRoleNameRunner = Config.Bind(customRoleNameRunnerDef, "Runner");
            customRoleNameTank = Config.Bind(customRoleNameTankDef, "Tank");
            customRoleNameGambler = Config.Bind(customRoleNameGamblerDef, "Gambler");
            customRoleNameStrongman = Config.Bind(customRoleNameStrongmanDef, "Strongman");
            customRoleNameRL = Config.Bind(customRoleNameRLDef, "Ranged Looter");
            customRoleNameAthletic = Config.Bind(customRoleNameAthleticDef, "Athletic");
            customRoleNameMage = Config.Bind(customRoleNameMageDef, "Mage");
            customRoleNameReaper = Config.Bind(customRoleNameReaperDef, "Reaper");
            customRoleNameScout = Config.Bind(customRoleNameScoutDef, "Scout");

            customRoleDecRunner = Config.Bind(customRoleDesRunnerDef, "You have more stamina and run much faster than everyone else!");
            customRoleDecTank = Config.Bind(customRoleDesTankDef, "You walk slower but your hp is doubled!");
            customRoleDecGambler = Config.Bind(customRoleDesGamblerDef, "You rolled random effects:");
            customRoleDecStrongman = Config.Bind(customRoleDesStrongmanDef, "You´re incredibly strong!");
            customRoleDecRL = Config.Bind(customRoleDesRLDef, "You can reach objects from far away!");
            customRoleDecAthletic = Config.Bind(customRoleDesAthleticDef, "You have more stamina, strength and can jump higher");
            customRoleDecMage = Config.Bind(customRoleDesMageDef, "You are able to use your mana to become incredibly strong!");
            customRoleDecReaper = Config.Bind(customRoleDesReaperDef, "For each enemy you and your friends kill, you become stronger!");
            customRoleDecScout = Config.Bind(customRoleDesScoutDef, "Your stamina is more efficient and by pressing [G] you can see all enemies around you.");

            scoutKey = Config.Bind(scoutButtonDef, KeyCode.G);

            harmony.PatchAll(typeof(PunManagerPatch));
            harmony.PatchAll(typeof(PlayerAvatarPatch));
            harmony.PatchAll(typeof(StatsManagerPatch));


            REPOLib.BundleLoader.LoadBundle(getPath(), assetBundle =>
            {
                var item = assetBundle.LoadAsset<Item>("Mana Regeneration Upgrade");
                REPOLib.Modules.Items.RegisterItem(item);
                var item2 = assetBundle.LoadAsset<Item>("Scout Cooldown Upgrade");
                REPOLib.Modules.Items.RegisterItem(item2);
            });
        }

        public static void Update_ManaRegeneration()
        {
            bool flag = !LevelGenerator.Instance.Generated || SemiFunc.MenuLevel();
            if (!flag)
            {
                int rechargeLevel = Upgrader.GetStat(PlayerAvatar.instance.steamID, "playerUpgradeManaRegeneration");
                guiManager.manaRegenRate = Upgrader.GetStat(PlayerAvatar.instance.steamID, "playerUpgradeManaRegeneration");
            }
        }

        public static void Update_ScoutCooldown()
        {
            bool flag = !LevelGenerator.Instance.Generated || SemiFunc.MenuLevel();
            if (!flag)
            {
                int rechargeLevel = Upgrader.GetStat(PlayerAvatar.instance.steamID, "playerUpgradeScoutCooldownReduction");
                ScoutMarker.reductionUpgrades = Upgrader.GetStat(PlayerAvatar.instance.steamID, "playerUpgradeScoutCooldownReduction");
            }
        }
    }

    [HarmonyPatch(typeof(PunManager))]
    static class PunManagerPatch
    {
        [HarmonyPostfix, HarmonyPatch(nameof(PunManager.Start))]
        private static void Start_Postfix(PunManager __instance)
        {
            if (__instance.GetComponent<StrengthManager>() == null)
            {
                __instance.AddComponent<StrengthManager>();
            }

            if (__instance.GetComponent<HealthManager>() == null)
            {
                __instance.AddComponent<HealthManager>();
            }

            if (__instance.GetComponent<ScoutMarker>() == null)
            {
                __instance.AddComponent<ScoutMarker>();
            }
        }
    }
}

public class ClassManager
{
    private System.Random rnd = new System.Random();

    public int chosenRoleId;
    private StrengthManager strManager = new StrengthManager();

    public String chosenRole;
    public int roleAmount = 9;

    static internal ManualLogSource mls;

    public RepoRoles repoRoles = new RepoRoles();
    public ReaperManager rMan;

    public readonly Harmony harmonyPatcher = new Harmony("patches.reporoles.mod");

    // Runner Params
    private float origMoveSpeed;
    private float origSprintSpeed;
    private float origCrouchSpeed;
    private float speedMultiplier;

    // Tank Params
    private int origPlayerHealth;
    private int origMaxPlayerHealth;

    // Other Params
    private float origJumpForce;
    private float origMaxEnergy;
    private float origGrabStrength;
    private float origGrabRange;
    public static int stackKills = 0;
    public static bool isTank;
    public static bool isScout;

    private string[] roleNames = ["Random", "Runner", "Tank", "Gambler", "Strongman", "Ranged Looter", "Athletic", "Mage", "Reaper", "Scout"];



    public ClassManager()
    {
        if (mls == null)
        {
            mls = BepInEx.Logging.Logger.CreateLogSource("ClassManager");
        }
        
        origMoveSpeed = PlayerController.instance.MoveSpeed;
        origSprintSpeed = PlayerController.instance.SprintSpeed;
        origCrouchSpeed = PlayerController.instance.CrouchSpeed;
        speedMultiplier = 1.5f;

        origJumpForce = PlayerController.instance.JumpForce;

        origPlayerHealth = (int) AccessTools.Field(typeof(PlayerHealth), "health").GetValue(PlayerAvatar.instance.playerHealth);
        origMaxPlayerHealth = (int) AccessTools.Field(typeof(PlayerHealth), "maxHealth").GetValue(PlayerAvatar.instance.playerHealth);

        origMaxEnergy = PlayerController.instance.EnergyStart;
        origGrabStrength = PhysGrabber.instance.grabStrength;

        origGrabRange = PhysGrabber.instance.grabRange;
    }

    public int genGamblerEffectNr()
    {
        int effectnr = rnd.Next(0,5);
        return effectnr;
    }

    public object[] genGamblerEffects()
    {
        string[] posEffectArray = ["You walk faster", "You have more stamina", "You have more health", "You´re stronger", "You jump higher"];
        string[] negEffectArray = ["you walk slower", "you have less stamina", "you have less health", "you´re weaker", "you don´t jump as high"];

        int posEffectNr = genGamblerEffectNr();
        int negEffectNr = genGamblerEffectNr();

        while (posEffectNr == negEffectNr)
        {
            posEffectNr = genGamblerEffectNr();
            negEffectNr = genGamblerEffectNr();
        }

        string posEffect = posEffectArray[posEffectNr];
        string negEffect = negEffectArray[negEffectNr];


        string fullString = RepoRoles.customRoleDecGambler.Value + " " + posEffect + " but " + negEffect + "!";

        object[] returnArray = [fullString, posEffectNr, negEffectNr];

        return returnArray;
    }

    public void assignRoleFromConfig(PlayerController __instance)
    {
        string roleChoice = RepoRoles.savedRole.Value;
        if (roleChoice == "" || roleChoice == "Random")
        {
            chosenRoleId = rnd.Next(1, roleAmount+1);
            mls.LogInfo("Rolling random role...");
        }
        else
        {
            chosenRoleId = Array.IndexOf(roleNames, roleChoice);
        }

        assignRole(chosenRoleId, __instance);
    }

    private void modifyStrength(string steamID, float newStrength)
    {
        if (!SemiFunc.IsMultiplayer())
        {
            PlayerAvatar.instance.physGrabber.grabStrength = newStrength;
        }
        else
        {
            PunManager.instance.photonView.RPC("setStrengthRPC", RpcTarget.All, steamID, newStrength);
        }
    }

    private void setHealth(string steamID, int maxHealth, int health)
    {
        if (!SemiFunc.IsMultiplayer())
        {
            PlayerAvatar.instance.playerHealth.health = health;
            PlayerAvatar.instance.playerHealth.maxHealth = maxHealth;
        }
        else
        {
            PunManager.instance.photonView.RPC("setHealthRPC", RpcTarget.All, steamID, maxHealth, health);
        }
    }

    public void setReaperStatus(string steamID, bool isReaper)
    {
        if (!SemiFunc.IsMultiplayer())
        {
            PlayerAvatar.instance.GetComponent<ReaperManager>().isReaper = isReaper;
        }
        else if(PlayerAvatar.instance.photonView != null)
        {
            PlayerAvatar.instance.photonView.RPC("setReaperStatusRPC", RpcTarget.All, steamID, isReaper);
        }
    }

    public void assignRole(int roleId, PlayerController __instance)
    {
        RepoRoles.Update_ManaRegeneration();
        RepoRoles.Update_ScoutCooldown();

        if (PlayerAvatar.instance.GetComponent<ReaperManager>() != null)
        {
            rMan = PlayerAvatar.instance.GetComponent<ReaperManager>();
        }
        else
        {
            Debug.LogError("Failed to get Reaper Manager! Please contact the mod developer about this.");
        }

        // getting upgrades --------------------------------------
        var playerName = SemiFunc.PlayerGetName(PlayerAvatar.instance);
        var playerSteamID = SemiFunc.PlayerGetSteamID(PlayerAvatar.instance);
        var numberOfHealthUpgrades = 0;
        var numberOfSpeedUpgrades = 0;
        var numberOfStaminaUpgrades = 0;
        var numberOfStrengthUpgrades = 0;
        var numberOfRangeUpgrades = 0;

        if (StatsManager.instance.playerUpgradeHealth.TryGetValue(playerSteamID, out var upgradeLevel))
        {
            numberOfHealthUpgrades = upgradeLevel;
        }

        if (StatsManager.instance.playerUpgradeSpeed.TryGetValue(playerSteamID, out var upgradeLevel2))
        {
            numberOfSpeedUpgrades = upgradeLevel2;
        }

        if (StatsManager.instance.playerUpgradeStamina.TryGetValue(playerSteamID, out var upgradeLevel3))
        {
            numberOfStaminaUpgrades = upgradeLevel3;
        }

        if (StatsManager.instance.playerUpgradeStrength.TryGetValue(playerSteamID, out var upgradeLevel4))
        {
            numberOfStrengthUpgrades = upgradeLevel4;
        }

        if (StatsManager.instance.playerUpgradeRange.TryGetValue(playerSteamID, out var upgradeLevel5))
        {
            numberOfRangeUpgrades = upgradeLevel5;
        }

        stackKills = 0;

        __instance.CrouchSpeed = origCrouchSpeed; // -
        __instance.MoveSpeed = origMoveSpeed; // -
        __instance.SprintSpeed = origSprintSpeed + numberOfSpeedUpgrades * 1f; // +

        __instance.JumpForce = origJumpForce; // -

        __instance.EnergyStart = origMaxEnergy + numberOfStaminaUpgrades * 10f; // +
        __instance.EnergyCurrent = origMaxEnergy + numberOfStaminaUpgrades * 10f; // -

        PlayerAvatar.instance.physGrabber.grabRange = origGrabRange + 1f * numberOfRangeUpgrades;
        modifyStrength(PlayerController.instance.playerSteamID, origGrabStrength + numberOfStrengthUpgrades * 0.2f);

        int maxOrigHP = origMaxPlayerHealth + numberOfHealthUpgrades * 20;
        int actualHPModified = (int) AccessTools.Field(typeof(PlayerHealth), "health").GetValue(PlayerAvatar.instance.playerHealth);

        if ((int) AccessTools.Field(typeof(PlayerHealth), "health").GetValue(PlayerAvatar.instance.playerHealth) > maxOrigHP) // over cap?
        {
            setHealth(PlayerController.instance.playerSteamID, maxOrigHP, maxOrigHP);
        }
        else
        {
            setHealth(PlayerController.instance.playerSteamID, maxOrigHP, actualHPModified);
        }

        guiManager.isMage = false;
        isTank = false;
        setReaperStatus(PlayerController.instance.playerSteamID, false);
        isScout = false;

        if (!SemiFunc.RunIsLevel() || SemiFunc.RunIsShop())
            return;
        


        if(!RepoRoles.enableRunner.Value && !RepoRoles.enableTank.Value && !RepoRoles.enableGambler.Value && !RepoRoles.enableStrongman.Value && !RepoRoles.enableRL.Value && !RepoRoles.enableAthletic.Value && !RepoRoles.enableMage.Value && !RepoRoles.enableReaper.Value && !RepoRoles.enableScout.Value)
        {
            Debug.LogError("WARNING! You disabled all roles in the config file. You will not get any roles until you change it back.");
            return;
        }

        if (RepoRoles.showGUIAtStart.Value == true)
            guiManager.showGUI = true;
        else
            guiManager.showGUI = false;

        harmonyPatcher.PatchAll(typeof(PunManagerPatch));

        if (roleId <= 0)
        {
            mls.LogWarning("Unable to find RoleId! Please contact the mod developer.");
        }

        else if (roleId == 1)
        {
            if (!RepoRoles.enableRunner.Value)
            {
                assignRole(rnd.Next(1, roleAmount + 1), PlayerController.instance);
                Debug.Log("You got assigned a new random role because this one was disabled.");
                return;
            }
            mls.LogInfo("Assigning role " + RepoRoles.customRoleNameRunner.Value + ".");

            __instance.CrouchSpeed = origCrouchSpeed * speedMultiplier;
            __instance.MoveSpeed = origMoveSpeed * speedMultiplier;
            __instance.SprintSpeed = origSprintSpeed * speedMultiplier + numberOfSpeedUpgrades * 1f;

            __instance.EnergyStart = (origMaxEnergy + numberOfStaminaUpgrades * 10f) * 1.2f;
            __instance.EnergyCurrent = (origMaxEnergy + numberOfStaminaUpgrades * 10f) * 1.2f;


            RepoRoles.GUIinstance.text = RepoRoles.customRoleNameRunner.Value;
            RepoRoles.GUIinstance.color = new Color(0.973f, 1f, 0.196f);

            RepoRoles.GUIinstance.descText = RepoRoles.customRoleDecRunner.Value + "\nPress " + RepoRoles.toggleKey.Value + " to continue";
            RepoRoles.GUIinstance.descColor = new Color(0.973f, 1f, 0.196f);
        }

        else if (roleId == 2)
        {
            if (!RepoRoles.enableTank.Value)
            {
                assignRole(rnd.Next(1, roleAmount + 1), PlayerController.instance);
                Debug.Log("You got assigned a new random role because this one was disabled.");
                return;
            }

            mls.LogInfo("Assigning role " + RepoRoles.customRoleNameTank.Value + ".");

            int maxHP = (int)AccessTools.Field(typeof(PlayerHealth), "maxHealth").GetValue(PlayerAvatar.instance.playerHealth);
            int actualHP = (int)AccessTools.Field(typeof(PlayerHealth), "health").GetValue(PlayerAvatar.instance.playerHealth);

            setHealth(PlayerController.instance.playerSteamID, maxHP * 2, actualHP * 2);



            __instance.CrouchSpeed = origCrouchSpeed * 0.9f;
            __instance.MoveSpeed = origMoveSpeed * 0.9f;
            __instance.SprintSpeed = (origSprintSpeed + numberOfSpeedUpgrades * 1f) * 0.9f;

            isTank = true;

            RepoRoles.GUIinstance.text = RepoRoles.customRoleNameTank.Value;
            RepoRoles.GUIinstance.color = Color.gray;

            RepoRoles.GUIinstance.descText = RepoRoles.customRoleDecTank.Value + "\nPress " + RepoRoles.toggleKey.Value + " to continue";
            RepoRoles.GUIinstance.descColor = Color.gray;
        }

        else if (roleId == 3)
        {
            if (!RepoRoles.enableGambler.Value)
            {
                assignRole(rnd.Next(1, roleAmount + 1), PlayerController.instance);
                Debug.Log("You got assigned a new random role because this one was disabled.");
                return;
            }

            mls.LogInfo("Assigning role " + RepoRoles.customRoleNameGambler.Value + ".");

            object[] values = genGamblerEffects();

            if ((int) values[1] == 0)
            {
                __instance.CrouchSpeed = origCrouchSpeed * 1.3f;
                __instance.MoveSpeed = origMoveSpeed * 1.3f;
                __instance.SprintSpeed = origSprintSpeed + numberOfSpeedUpgrades * 1.3f;
            }
            else if ((int)values[1] == 1)
            {
                __instance.EnergyStart = (origMaxEnergy + numberOfStaminaUpgrades * 10f) * 1.8f;
                __instance.EnergyCurrent = (origMaxEnergy + numberOfStaminaUpgrades * 10f) * 1.8f;
            }
            else if ((int)values[1] == 2)
            {
                int maxHP = (int)AccessTools.Field(typeof(PlayerHealth), "maxHealth").GetValue(PlayerAvatar.instance.playerHealth);
                int actualHP = (int)AccessTools.Field(typeof(PlayerHealth), "health").GetValue(PlayerAvatar.instance.playerHealth);

                setHealth(PlayerController.instance.playerSteamID, (int)(maxHP * 1.8), (int)(actualHP * 1.8));
            }
            else if ((int)values[1] == 3)
            {
                modifyStrength(PlayerController.instance.playerSteamID, (origGrabStrength + numberOfStrengthUpgrades * 0.2f) * 1.3f);
            }
            else if ((int)values[1] == 4)
            {
                __instance.JumpForce = __instance.JumpForce * 1.5f;
            }

            if ((int) values[2] == 0)
            {
                __instance.CrouchSpeed = origCrouchSpeed; // -
                __instance.MoveSpeed = origMoveSpeed; // -
                __instance.SprintSpeed = origSprintSpeed + numberOfSpeedUpgrades * 0.8f;
            }
            else if ((int)values[2] == 1)
            {
                __instance.EnergyStart = (origMaxEnergy + numberOfStaminaUpgrades * 10f) * 0.8f;
                __instance.EnergyCurrent = (origMaxEnergy + numberOfStaminaUpgrades * 10f) * 0.8f;
            }
            else if ((int)values[2] == 2)
            {
                int maxHP = (int)AccessTools.Field(typeof(PlayerHealth), "maxHealth").GetValue(PlayerAvatar.instance.playerHealth);
                int actualHP = (int)AccessTools.Field(typeof(PlayerHealth), "health").GetValue(PlayerAvatar.instance.playerHealth);

                setHealth(PlayerController.instance.playerSteamID, (int)(maxHP * 0.8), (int)(actualHP * 0.8));
            }
            else if ((int)values[2] == 3)
            {
                modifyStrength(PlayerController.instance.playerSteamID, (origGrabStrength + numberOfStrengthUpgrades * 0.2f) * 0.8f);
            }
            else if ((int)values[2] == 4)
            {
                __instance.JumpForce = __instance.JumpForce * 0.7f;
            }

            RepoRoles.GUIinstance.text = RepoRoles.customRoleNameGambler.Value;
            RepoRoles.GUIinstance.color = new Color(0.576f, 0f, 0.831f);

            RepoRoles.GUIinstance.descColor = new Color(0.576f, 0f, 0.831f);
            RepoRoles.GUIinstance.descText = values[0] + "\nPress " + RepoRoles.toggleKey.Value + " to continue";
        }

        else if (roleId == 4)
        {
            if (!RepoRoles.enableStrongman.Value)
            {
                assignRole(rnd.Next(1, roleAmount + 1), PlayerController.instance);
                Debug.Log("You got assigned a new random role because this one was disabled.");
                return;
            }

            mls.LogMessage("Assigning role " + RepoRoles.customRoleNameStrongman.Value + ".");
            Debug.Log("Strength before: " + PhysGrabber.instance.grabStrength);

            modifyStrength(PlayerController.instance.playerSteamID, (origGrabStrength + numberOfStrengthUpgrades * 0.2f) * 1.5f + 0.5f);

            Debug.Log("Strength after: " + PhysGrabber.instance.grabStrength);

            RepoRoles.GUIinstance.text = RepoRoles.customRoleNameStrongman.Value;
            RepoRoles.GUIinstance.color = new Color(0.761f, 0.055f, 0.055f);

            RepoRoles.GUIinstance.descText = RepoRoles.customRoleDecStrongman.Value + "\nPress " + RepoRoles.toggleKey.Value + " to continue";
            RepoRoles.GUIinstance.descColor = new Color(0.761f, 0.055f, 0.055f);
        }

        else if (roleId == 5)
        {
            if (!RepoRoles.enableRL.Value)
            {
                assignRole(rnd.Next(1, roleAmount + 1), PlayerController.instance);
                Debug.Log("You got assigned a new random role because this one was disabled.");
                return;
            }

            mls.LogMessage("Assigning role " + RepoRoles.customRoleNameRL.Value + ".");

            PhysGrabber.instance.grabRange = (origGrabRange + numberOfRangeUpgrades * 1f) * 2.5f;
            modifyStrength(PlayerController.instance.playerSteamID, (origGrabStrength + numberOfStrengthUpgrades * 0.2f) * 1.2f);

            RepoRoles.GUIinstance.text = RepoRoles.customRoleNameRL.Value;
            RepoRoles.GUIinstance.color = new Color(0.592f, 0.969f, 0.663f);

            RepoRoles.GUIinstance.descText = "You can reach objects from far away!\nPress " + RepoRoles.toggleKey.Value + " to continue";
            RepoRoles.GUIinstance.descColor = new Color(0.592f, 0.969f, 0.663f);
        }

        else if (roleId == 6)
        {
            if (!RepoRoles.enableAthletic.Value)
            {
                assignRole(rnd.Next(1, roleAmount + 1), PlayerController.instance);
                Debug.Log("You got assigned a new random role because this one was disabled.");
                return;
            }

            mls.LogMessage("Assigning role " + RepoRoles.customRoleNameAthletic.Value + ".");

            modifyStrength(PlayerController.instance.playerSteamID, (origGrabStrength + numberOfStrengthUpgrades * 0.2f) * 1.3f);

            __instance.EnergyStart = origMaxEnergy + numberOfStaminaUpgrades * 10f + 20;
            __instance.EnergyCurrent = origMaxEnergy + numberOfStaminaUpgrades * 10f + 20;

            __instance.JumpForce = origJumpForce + 3f;

            RepoRoles.GUIinstance.text = RepoRoles.customRoleNameAthletic.Value;
            RepoRoles.GUIinstance.color = Color.white;

            RepoRoles.GUIinstance.descText = RepoRoles.customRoleDecAthletic.Value + "\nPress " + RepoRoles.toggleKey.Value + " to continue";
            RepoRoles.GUIinstance.descColor = Color.white;
        }

        else if (roleId == 7)
        {
            if (!RepoRoles.enableMage.Value)
            {
                assignRole(rnd.Next(1, roleAmount + 1), PlayerController.instance);
                Debug.Log("You got assigned a new random role because this one was disabled.");
                return;
            }

            mls.LogMessage("Assigning role " + RepoRoles.customRoleNameMage.Value + ".");

            guiManager.isMage = true;

            guiManager.aviableMana = 8;

            ManaHelper.CreateUI();

            int maxHP = (int)AccessTools.Field(typeof(PlayerHealth), "maxHealth").GetValue(PlayerAvatar.instance.playerHealth);
            int actualHP = (int)AccessTools.Field(typeof(PlayerHealth), "health").GetValue(PlayerAvatar.instance.playerHealth);

            if((int) actualHP * 0.5 > 0)
            {
                setHealth(PlayerController.instance.playerSteamID, (int)(maxHP * 0.5), (int)(actualHP * 0.5));
            }
            else
            {
                setHealth(PlayerController.instance.playerSteamID, (int)(maxHP * 0.5), actualHP);
            }

            RepoRoles.GUIinstance.text = RepoRoles.customRoleNameMage.Value;
            RepoRoles.GUIinstance.color = new Color(0f, 0.384f, 1f);

            RepoRoles.GUIinstance.descText = RepoRoles.customRoleDecMage.Value + "\nPress " + RepoRoles.showSpellsKey.Value + " to see all your spells and press " + RepoRoles.toggleKey.Value + " to continue";
            RepoRoles.GUIinstance.descColor = new Color(0f, 0.384f, 1f);
        }

        else if (roleId == 8)
        {
            if (!RepoRoles.enableReaper.Value)
            {
                assignRole(rnd.Next(1, roleAmount + 1), PlayerController.instance);
                Debug.Log("You got assigned a new random role because this one was disabled.");
                return;
            }

            mls.LogMessage("Assigning role " + RepoRoles.customRoleNameReaper.Value + ".");

            harmonyPatcher.PatchAll(typeof(ReaperPatch));
            setReaperStatus(PlayerController.instance.playerSteamID, true);

            int maxHP = (int)AccessTools.Field(typeof(PlayerHealth), "maxHealth").GetValue(PlayerAvatar.instance.playerHealth);
            int actualHP = (int)AccessTools.Field(typeof(PlayerHealth), "health").GetValue(PlayerAvatar.instance.playerHealth);

            setHealth(PlayerController.instance.playerSteamID, (int)(maxHP * 1.5), (int)(actualHP * 1.5));

            RepoRoles.GUIinstance.text = RepoRoles.customRoleNameReaper.Value;
            RepoRoles.GUIinstance.color = new Color(0.141f, 0.6f, 0.502f);

            RepoRoles.GUIinstance.descText = RepoRoles.customRoleDecReaper.Value + "\nPress " + RepoRoles.toggleKey.Value + " to continue";
            RepoRoles.GUIinstance.descColor = new Color(0.141f, 0.6f, 0.502f);
        }

        else if (roleId == 9)
        {
            if (!RepoRoles.enableScout.Value)
            {
                assignRole(rnd.Next(1, roleAmount + 1), PlayerController.instance);
                Debug.Log("You got assigned a new random role because this one was disabled.");
                return;
            }

            mls.LogMessage("Assigning role " + RepoRoles.customRoleNameReaper.Value + ".");

            isScout = true;

            __instance.EnergyStart = (origMaxEnergy + numberOfStaminaUpgrades * 10f) * 2f;
            __instance.EnergyCurrent = (origMaxEnergy + numberOfStaminaUpgrades * 10f) * 2f;
            __instance.sprintRechargeAmount *= 2;

            RepoRoles.GUIinstance.text = RepoRoles.customRoleNameScout.Value;
            RepoRoles.GUIinstance.color = new Color(0.902f, 0.733f, 0.11f);

            RepoRoles.GUIinstance.descText = RepoRoles.customRoleDecScout.Value.Replace("[G]", "[" + RepoRoles.scoutKey.Value + "]") + "\nPress " + RepoRoles.toggleKey.Value + " to continue";
            RepoRoles.GUIinstance.descColor = new Color(0.902f, 0.733f, 0.11f);
        }

    }
}

public class guiManager : MonoBehaviour
{
    public static bool showGUI = true;
    public static bool isMage = false;
    private static bool showSpells = false;

    private bool isDead = false;

    public string text;
    public Color color = Color.white;

    public string descText;
    public Color descColor = Color.white;
    
    private Texture2D _backgroundTexture;
    public static Texture2D manaTexture;
    public static Font customFont;
    private static AssetBundle fontBundle;

    public static int aviableMana = 0;
    public static int manaRegenRate = 0;
    public int speedTicker = 0;
    public int manaTicks = 0;
    public bool speedActive;

    float multiSpeed = 1.4f;
    float crouchSpeedBefore;
    float moveSpeedBefore;
    float sprintSpeedBefore;
    int overchargeMulti = 1;
    bool isOvercharged = false;
    int overchargeTicker = 0;
    bool jumpActive = false;
    float jumpOriginal;
    int jumpTicker = 0;

    int neededJumpTicker = 1800;
    int neededSpeedTicker = 1800;

    int manaUsageTicker = 0;
    bool showManaUsage = false;
    Texture2D textureRunner;

    string manaUsageText = "Used x (- y Mana)";

    private bool fontLoaded = false;
    private bool RunnerTextureLoaded = false;


    public static AssetBundle LoadAssetBundle(string name)
    {
        BepInEx.Logging.Logger.CreateLogSource("AssetBundles").LogDebug("Loading Asset Bundle: " + name);
        AssetBundle bundle = null;
        string path = Path.Combine(Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location), name);
        bundle = AssetBundle.LoadFromFile(path);
        return bundle;
    }

    public static Texture2D LoadBottleTextureFromFile()
    {
        AssetBundle assetBundle = LoadAssetBundle("REPORoles_assets1");
        Texture2D texture = assetBundle.LoadAsset<Texture2D>("emptyBottle.png");
        assetBundle.Unload(false);
        return texture;
    }

    public static Texture2D LoadFullBottleTextureFromFile()
    {
        AssetBundle assetBundle = LoadAssetBundle("REPORoles_assets1");
        Texture2D texture = assetBundle.LoadAsset<Texture2D>("filledBottle.png");
        assetBundle.Unload(false);
        return texture;
    }

    public static Texture2D LoadTextureByName(string bundleName, string assetName)
    {
        AssetBundle assetBundle = LoadAssetBundle(bundleName);
        Texture2D texture = assetBundle.LoadAsset<Texture2D>(assetName);
        assetBundle.Unload(false);
        return texture;
    }

    public static void loadFont()
    {
        if (fontBundle == null)
        {
            fontBundle = LoadAssetBundle("REPORoles_assets1");
            customFont = fontBundle.LoadAsset<Font>("font.ttf");
        }
        if (fontBundle == null)
        {
            BepInEx.Logging.Logger.CreateLogSource("RepoRoles ERROR").LogError("Missing Assets: Put the assets file next to the dll in your plugins folder. If this does not fix it contact us on our discord (see thunderstore description or README)");
        }
        fontBundle.Unload(false);
    }


    private void Start()
    {
        _backgroundTexture = LoadBottleTextureFromFile();
        _backgroundTexture.Apply();
        manaTexture = LoadFullBottleTextureFromFile();
        manaTexture.Apply();
    }

    private void OnGUI()
    {
        float x = Screen.width / 2 - 100;
        float y = Screen.height / 2 - 60;

        if (!fontLoaded)
        {
            loadFont();
            fontLoaded = true;
        }

        if (SemiFunc.RunIsLevel() && !SemiFunc.RunIsShop() && showGUI)
        {
            GUIStyle textStyle = new GUIStyle();
            textStyle.fontSize = 60;
            textStyle.fontStyle = FontStyle.Bold;
            textStyle.richText = true;
            textStyle.normal.textColor = color; // color of the text
            textStyle.alignment = TextAnchor.MiddleCenter;
            textStyle.font = customFont;

            GUIStyle outlineStyle = new GUIStyle(textStyle);
            outlineStyle.normal.textColor = Color.black; // outline color
            outlineStyle.alignment = TextAnchor.MiddleCenter;
            outlineStyle.font = customFont;


            float outlineSize = 2f; // outline

            // render 8 times
            GUI.Label(new Rect(x - outlineSize, y, 200, 50), text, outlineStyle);
            GUI.Label(new Rect(x + outlineSize, y, 200, 50), text, outlineStyle);
            GUI.Label(new Rect(x, y - outlineSize, 200, 50), text, outlineStyle);
            GUI.Label(new Rect(x, y + outlineSize, 200, 50), text, outlineStyle);

            GUI.Label(new Rect(x - outlineSize, y - outlineSize, 200, 50), text, outlineStyle);
            GUI.Label(new Rect(x - outlineSize, y + outlineSize, 200, 50), text, outlineStyle);
            GUI.Label(new Rect(x + outlineSize, y - outlineSize, 200, 50), text, outlineStyle);
            GUI.Label(new Rect(x + outlineSize, y + outlineSize, 200, 50), text, outlineStyle);

            // main text
            GUI.Label(new Rect(x, y, 200, 50), text, textStyle);

            //-------------------------------
            //Texture2D textureRunner;
            //GUIStyle runnerImageStyle = new GUIStyle();
            //if (!RunnerTextureLoaded)
            //{
            //    textureRunner = LoadTextureByName("REPORoles_assets1", "runner.png");
            //    RunnerTextureLoaded = true;
            //}
            //runnerImageStyle.alignment = TextAnchor.MiddleCenter;
            //runnerImageStyle.font = customFont;
            //
            //GUI.Box(new Rect(x + 250, 50, 50, 50), GUIContent.none, runnerImageStyle);

            //-------------------------------

            float x2 = Screen.width / 2 - 100;
            float y2 = Screen.height / 2 + 40;

            GUIStyle descTextStyle = new GUIStyle();
            descTextStyle.fontSize = 40;
            descTextStyle.fontStyle = FontStyle.Bold;
            descTextStyle.richText = true;
            descTextStyle.normal.textColor = color; // color of the text
            descTextStyle.alignment = TextAnchor.MiddleCenter;
            descTextStyle.font = customFont;

            GUIStyle descOutlineStyle = new GUIStyle(descTextStyle);
            descOutlineStyle.normal.textColor = Color.black; // outline color
            descOutlineStyle.alignment = TextAnchor.MiddleCenter;
            descOutlineStyle.font = customFont;


            float descOutlineSize = 2f; // outline

            // render 8 times
            GUI.Label(new Rect(x2 - descOutlineSize, y2, 200, 50), descText, descOutlineStyle);
            GUI.Label(new Rect(x2 + descOutlineSize, y2, 200, 50), descText, descOutlineStyle);
            GUI.Label(new Rect(x2, y2 - descOutlineSize, 200, 50), descText, descOutlineStyle);
            GUI.Label(new Rect(x2, y2 + descOutlineSize, 200, 50), descText, descOutlineStyle);

            GUI.Label(new Rect(x2 - descOutlineSize, y2 - descOutlineSize, 200, 50), descText, descOutlineStyle);
            GUI.Label(new Rect(x2 - descOutlineSize, y2 + descOutlineSize, 200, 50), descText, descOutlineStyle);
            GUI.Label(new Rect(x2 + descOutlineSize, y2 - descOutlineSize, 200, 50), descText, descOutlineStyle);
            GUI.Label(new Rect(x2 + descOutlineSize, y2 + descOutlineSize, 200, 50), descText, descOutlineStyle);

            // main text
            GUI.Label(new Rect(x2, y2, 200, 50), descText, descTextStyle);
        }


        if (!isMage || !SemiFunc.RunIsLevel() || SemiFunc.RunIsShop() || (int) AccessTools.Field(typeof(PlayerHealth), "health").GetValue(PlayerAvatar.instance.playerHealth) <= 0)
            return;

        GUIStyle manaStyle = new GUIStyle();
        manaStyle.alignment = TextAnchor.UpperCenter;
        manaStyle.normal.background = manaTexture;
        manaStyle.font = customFont;

        GUIStyle backgroundStyle = new GUIStyle();
        backgroundStyle.alignment = TextAnchor.UpperCenter;
        backgroundStyle.normal.background = _backgroundTexture;
        backgroundStyle.font = customFont;

        if (showManaUsage == true && SemiFunc.RunIsLevel() && !SemiFunc.RunIsShop() && isMage)
        {
            int xManaUsage = Screen.width / 2 - 100;
            int yManaUsage = Screen.height - 125;

            GUIStyle manaUsageStyle = new GUIStyle();
            manaUsageStyle.fontSize = 20;
            manaUsageStyle.fontStyle = FontStyle.Bold;
            manaUsageStyle.richText = true;
            manaUsageStyle.normal.textColor = new Color(0f, 0.384f, 1f); // color of the text
            manaUsageStyle.alignment = TextAnchor.MiddleCenter;
            manaUsageStyle.font = customFont;

            GUIStyle manaUsageOutlineStyle = new GUIStyle(manaUsageStyle);
            manaUsageOutlineStyle.normal.textColor = Color.black; // outline color
            manaUsageOutlineStyle.alignment = TextAnchor.MiddleCenter;
            manaUsageOutlineStyle.font = customFont;


            float manaUsageOutlineSize = 1.5f; // outline

            // render 8 times
            GUI.Label(new Rect(xManaUsage - manaUsageOutlineSize, yManaUsage, 200, 50), manaUsageText, manaUsageOutlineStyle);
            GUI.Label(new Rect(xManaUsage + manaUsageOutlineSize, yManaUsage, 200, 50), manaUsageText, manaUsageOutlineStyle);
            GUI.Label(new Rect(xManaUsage, yManaUsage - manaUsageOutlineSize, 200, 50), manaUsageText, manaUsageOutlineStyle);
            GUI.Label(new Rect(xManaUsage, yManaUsage + manaUsageOutlineSize, 200, 50), manaUsageText, manaUsageOutlineStyle);

            GUI.Label(new Rect(xManaUsage - manaUsageOutlineSize, yManaUsage - manaUsageOutlineSize, 200, 50), manaUsageText, manaUsageOutlineStyle);
            GUI.Label(new Rect(xManaUsage - manaUsageOutlineSize, yManaUsage + manaUsageOutlineSize, 200, 50), manaUsageText, manaUsageOutlineStyle);
            GUI.Label(new Rect(xManaUsage + manaUsageOutlineSize, yManaUsage - manaUsageOutlineSize, 200, 50), manaUsageText, manaUsageOutlineStyle);
            GUI.Label(new Rect(xManaUsage + manaUsageOutlineSize, yManaUsage + manaUsageOutlineSize, 200, 50), manaUsageText, manaUsageOutlineStyle);

            // main text
            GUI.Label(new Rect(xManaUsage, yManaUsage, 200, 50), manaUsageText, manaUsageStyle);
        }

        if (SemiFunc.RunIsLevel() && !SemiFunc.RunIsShop() && isMage && RepoRoles.mageTopManaBool == true)
        {
            if (ManaHelper.val == null)
                ManaHelper.val = GameObject.Find("Energy");

            GUI.Box(new Rect(x - 100, 50, 50, 50), GUIContent.none, backgroundStyle);
            GUI.Box(new Rect(x - 50, 50, 50, 50), GUIContent.none, backgroundStyle);
            GUI.Box(new Rect(x, 50, 50, 50), GUIContent.none, backgroundStyle);
            GUI.Box(new Rect(x + 50, 50, 50, 50), GUIContent.none, backgroundStyle);
            GUI.Box(new Rect(x + 100, 50, 50, 50), GUIContent.none, backgroundStyle);
            GUI.Box(new Rect(x + 150, 50, 50, 50), GUIContent.none, backgroundStyle);
            GUI.Box(new Rect(x + 200, 50, 50, 50), GUIContent.none, backgroundStyle);
            GUI.Box(new Rect(x + 250, 50, 50, 50), GUIContent.none, backgroundStyle);

            if (aviableMana >= 1)
            {
                GUI.Box(new Rect(x - 100, 50, 50, 50), GUIContent.none, manaStyle);
            }
            if (aviableMana >= 2)
            {
                GUI.Box(new Rect(x - 50, 50, 50, 50), GUIContent.none, manaStyle);
            }
            if (aviableMana >= 3)
            {
                GUI.Box(new Rect(x, 50, 50, 50), GUIContent.none, manaStyle);
            }
            if (aviableMana >= 4)
            {
                GUI.Box(new Rect(x + 50, 50, 50, 50), GUIContent.none, manaStyle);
            }
            if (aviableMana >= 5)
            {
                GUI.Box(new Rect(x + 100, 50, 50, 50), GUIContent.none, manaStyle);
            }
            if (aviableMana >= 6)
            {
                GUI.Box(new Rect(x + 150, 50, 50, 50), GUIContent.none, manaStyle);
            }
            if (aviableMana >= 7)
            {
                GUI.Box(new Rect(x + 200, 50, 50, 50), GUIContent.none, manaStyle);
            }
            if (aviableMana >= 8)
            {
                GUI.Box(new Rect(x + 250, 50, 50, 50), GUIContent.none, manaStyle);
            }
        }

        if (showSpells && SemiFunc.RunIsLevel() && !SemiFunc.RunIsShop())
        {
            GUIStyle descSpellStyle = new GUIStyle();
            descSpellStyle.fontSize = 40;
            descSpellStyle.fontStyle = FontStyle.Bold;
            descSpellStyle.richText = true;
            descSpellStyle.normal.textColor = new Color(0f, 0.384f, 1f); // color of the text
            descSpellStyle.alignment = TextAnchor.MiddleCenter;
            descSpellStyle.font = customFont;

            GUIStyle descSpellOutlineStyle = new GUIStyle(descSpellStyle);
            descSpellOutlineStyle.normal.textColor = Color.black; // outline color
            descSpellOutlineStyle.alignment = TextAnchor.MiddleCenter;
            descSpellOutlineStyle.font = customFont;

            string spellDescText = "<size=65>SPELLS</size>\n[" + RepoRoles.healKey.Value + "] Heal yourself for 5 health (1 Mana)\n\n[" + RepoRoles.staminaKey.Value + "] Regenerate your stamina to full (3 Mana)\n\n[" + RepoRoles.speedKey.Value + "] Become faster for 30 seconds (2 Mana)\n\n[" + RepoRoles.overchargeKey.Value + "] Boost the effectivity of spells used in the next 20 seconds (3 Mana)\n\n[" + RepoRoles.jumpKey.Value + "] Jump higher for 30 seconds (2 Mana)";
            

            float descSpellOutlineSize = 2f; // outline

            // render 8 times
            GUI.Label(new Rect(x - descSpellOutlineSize, y, 200, 50), spellDescText, descSpellOutlineStyle);
            GUI.Label(new Rect(x + descSpellOutlineSize, y, 200, 50), spellDescText, descSpellOutlineStyle);
            GUI.Label(new Rect(x, y - descSpellOutlineSize, 200, 50), spellDescText, descSpellOutlineStyle);
            GUI.Label(new Rect(x, y + descSpellOutlineSize, 200, 50), spellDescText, descSpellOutlineStyle);

            GUI.Label(new Rect(x - descSpellOutlineSize, y - descSpellOutlineSize, 200, 50), spellDescText, descSpellOutlineStyle);
            GUI.Label(new Rect(x - descSpellOutlineSize, y + descSpellOutlineSize, 200, 50), spellDescText, descSpellOutlineStyle);
            GUI.Label(new Rect(x + descSpellOutlineSize, y - descSpellOutlineSize, 200, 50), spellDescText, descSpellOutlineStyle);
            GUI.Label(new Rect(x + descSpellOutlineSize, y + descSpellOutlineSize, 200, 50), spellDescText, descSpellOutlineStyle);

            // main text
            GUI.Label(new Rect(x, y, 200, 50), spellDescText, descSpellStyle);
        }
    }

    private void updateMana()
    {
        if(isMage)
            ManaHelper.val2.gameObject.GetComponent<ManaUI>().SetMana(aviableMana, 8f);
    }

    private void Update()
    {
        if (SemiFunc.RunIsLevel() && (int)AccessTools.Field(typeof(PlayerHealth), "health").GetValue(PlayerAvatar.instance.playerHealth) <= 0)
        {
            isDead = true;
        }


        if (RepoRoles.mageTopManaBool && guiManager.isMage && ManaHelper.val != null && ManaHelper.val2 != null && ManaHelper.val2.activeInHierarchy)
        {
            ManaHelper.val2.SetActive(false);
        }
        else if (!RepoRoles.mageTopManaBool && guiManager.isMage && ManaHelper.val != null && ManaHelper.val2 != null && !ManaHelper.val2.activeInHierarchy)
        {
            ManaHelper.val2.SetActive(true);
            ManaHelper.val2.gameObject.GetComponent<ManaUI>().Text.enabled = true;
        }


        if (!SemiFunc.RunIsLevel())
        {
            isMage = false;
            if (Sender.manager != null && PlayerAvatar.instance != null)
                Sender.manager.setReaperStatus(PlayerController.instance.playerSteamID, false);
            ClassManager.isTank = false;
            isDead = false;
        }

        if (!RepoRoles.afterDeathNewRole)
        {
            isDead = false;
        }

        if (isDead && (int)AccessTools.Field(typeof(PlayerHealth), "health").GetValue(PlayerAvatar.instance.playerHealth) > 0 && RepoRoles.afterDeathNewRole)
        {
            Sender.manager.assignRoleFromConfig(PlayerController.instance);
            BepInEx.Logging.Logger.CreateLogSource("REPO Roles guiManager").LogInfo("Assigning new role on respawn!");
            isDead = false;
        }

        if (UnityEngine.Input.GetKeyDown(RepoRoles.toggleKey.Value) && !ChatManager.instance.chatActive)
        {
            showGUI = !showGUI;
        }

        if ((int)AccessTools.Field(typeof(PlayerHealth), "health").GetValue(PlayerAvatar.instance.playerHealth) > 0 && isMage && !ChatManager.instance.chatActive)
        {
            if (UnityEngine.Input.GetKeyDown(RepoRoles.showSpellsKey.Value))
            {
                showSpells = !showSpells;
            }

            if (UnityEngine.Input.GetKeyDown(RepoRoles.healKey.Value)) // Health Spell
            {
                if (((int)AccessTools.Field(typeof(PlayerHealth), "health").GetValue(PlayerAvatar.instance.playerHealth) + 5 * overchargeMulti) < ((int)AccessTools.Field(typeof(PlayerHealth), "maxHealth").GetValue(PlayerAvatar.instance.playerHealth)))
                {
                    if (aviableMana >= 1)
                    {
                        PlayerAvatar.instance.playerHealth.Heal(5 * overchargeMulti);
                        aviableMana = aviableMana - 1;
                        showManaUsage = true;
                        manaUsageTicker = 0;
                        manaUsageText = "Used Heal (-1 Mana)";
                        updateMana();
                    }
                }
                else if (!(((int)AccessTools.Field(typeof(PlayerHealth), "health").GetValue(PlayerAvatar.instance.playerHealth)) == ((int)AccessTools.Field(typeof(PlayerHealth), "maxHealth").GetValue(PlayerAvatar.instance.playerHealth))))
                {
                    if (aviableMana >= 1)
                    {
                        PlayerAvatar.instance.playerHealth.Heal((int) AccessTools.Field(typeof(PlayerHealth), "maxHealth").GetValue(PlayerAvatar.instance.playerHealth) - (int) AccessTools.Field(typeof(PlayerHealth), "health").GetValue(PlayerAvatar.instance.playerHealth));
                        aviableMana = aviableMana - 1;
                        showManaUsage = true;
                        manaUsageTicker = 0;
                        manaUsageText = "Used Heal (-1 Mana)";
                        updateMana();
                    }
                }
            }

            if (UnityEngine.Input.GetKeyDown(RepoRoles.speedKey.Value)) // Speed Spell
            {
                if (speedActive == false)
                {
                    crouchSpeedBefore = PlayerController.instance.CrouchSpeed;
                    moveSpeedBefore = PlayerController.instance.MoveSpeed;
                    sprintSpeedBefore = PlayerController.instance.SprintSpeed;

                    if (aviableMana >= 2)
                    {
                         
                        speedTicker = 0;
                        speedActive = true;

                        PlayerController.instance.CrouchSpeed = crouchSpeedBefore * multiSpeed;
                        PlayerController.instance.MoveSpeed = moveSpeedBefore * multiSpeed;
                        PlayerController.instance.SprintSpeed = sprintSpeedBefore * multiSpeed;
                        aviableMana = aviableMana - 2;
                        showManaUsage = true;
                        manaUsageTicker = 0;
                        manaUsageText = "Used Speed (-2 Mana)";
                        updateMana();
                        if (isOvercharged)
                        {
                            neededSpeedTicker = 3600;
                        }
                        else
                        {
                            neededSpeedTicker = 1800;
                        }
                    }
                }
            }

            if (UnityEngine.Input.GetKeyDown(RepoRoles.overchargeKey.Value)) // Overcharge Spell
            {
                if (isOvercharged == false)
                {
                    if (aviableMana >= 3)
                    {
                        aviableMana = aviableMana - 3;
                        isOvercharged = true;
                        overchargeMulti = 2;
                        overchargeTicker = 0;
                        showManaUsage = true;
                        manaUsageTicker = 0;
                        manaUsageText = "Used Overcharge (-3 Mana)";
                        updateMana();
                    }
                }
            }

            if (UnityEngine.Input.GetKeyDown(RepoRoles.jumpKey.Value)) // Speed Spell
            {
                if (jumpActive == false)
                {
                    jumpOriginal = PlayerController.instance.JumpForce;

                    if (aviableMana >= 2)
                    {
                        jumpTicker = 0;
                        jumpActive = true;

                        PlayerController.instance.JumpForce = jumpOriginal + 3f;
                        aviableMana = aviableMana - 2;

                        showManaUsage = true;
                        manaUsageTicker = 0;
                        manaUsageText = "Used Jump Boost (-2 Mana)";
                        updateMana();
                        if (isOvercharged)
                        {
                            neededJumpTicker = 3600;
                        }
                        else
                        {
                            neededJumpTicker = 1800;
                        }
                         
                    }
                }
            }

            if (UnityEngine.Input.GetKeyDown(RepoRoles.staminaKey.Value)) // Stamina Spell
            {
                if (PlayerController.instance.EnergyCurrent < PlayerController.instance.EnergyStart)
                {
                    if (aviableMana >= 3)
                    {
                        PlayerController.instance.EnergyCurrent = PlayerController.instance.EnergyStart;
                        aviableMana = aviableMana - 3;

                        showManaUsage = true;
                        manaUsageTicker = 0;
                        manaUsageText = "Used Stamina Refill (-3 Mana)";
                        updateMana();
                    }
                }
            }
        }

        if (speedActive == true && speedTicker >= neededSpeedTicker)
        {
            PlayerController.instance.CrouchSpeed = crouchSpeedBefore;
            PlayerController.instance.MoveSpeed = moveSpeedBefore;
            PlayerController.instance.SprintSpeed = sprintSpeedBefore;
            speedActive = false;
        }

        if (jumpActive == true && jumpTicker >= neededJumpTicker)
        {
            PlayerController.instance.JumpForce = jumpOriginal;
            jumpActive = false;
        }
    }
    private void setHealth(string steamID, int maxHealth, int health)
    {
        if (!SemiFunc.IsMultiplayer())
        {
            PlayerAvatar.instance.playerHealth.health = health;
            PlayerAvatar.instance.playerHealth.maxHealth = maxHealth;
        }
        else
        {
            PunManager.instance.photonView.RPC("setHealthRPC", RpcTarget.All, steamID, maxHealth, health);
        }
    }

    int passiveTankRegenTicker = 0;
    private void FixedUpdate()
    {
        if (passiveTankRegenTicker < 250 && ClassManager.isTank && SemiFunc.RunIsLevel() && !SemiFunc.RunIsShop() && !isDead)
        {
            passiveTankRegenTicker = passiveTankRegenTicker + 1;
        }

        if (passiveTankRegenTicker >= 250 && SemiFunc.RunIsLevel() && !SemiFunc.RunIsShop() && ClassManager.isTank)
        {
            passiveTankRegenTicker = 0;

            int maxPlayerHealth = (int)AccessTools.Field(typeof(PlayerHealth), "maxHealth").GetValue(PlayerAvatar.instance.playerHealth);
            int playerHealth = (int)AccessTools.Field(typeof(PlayerHealth), "health").GetValue(PlayerAvatar.instance.playerHealth);

            if (playerHealth < maxPlayerHealth) // over cap?
            {
                setHealth(PlayerController.instance.playerSteamID, maxPlayerHealth, playerHealth + 1);
            }
        }

        if (overchargeTicker >= 1200)
        {
            isOvercharged = false;
            overchargeMulti = 1;
        }

        if (isOvercharged == true)
        {
            overchargeTicker++;
        }

        manaUsageTicker++;
        speedTicker++;
        jumpTicker++;

        if (manaUsageTicker >= 420)
        {
            showManaUsage = false;
        }

        if (aviableMana < 8)
        {
            manaTicks++;
        }

        if (manaTicks >= 1300 - manaRegenRate * 120 && isMage && aviableMana < 8)
        {
            aviableMana++;
            manaTicks = 0;
            updateMana();
        }

    }
}

public class StrengthManager : MonoBehaviour
{
    internal PhotonView photonView;

    private void Start()
    {
        photonView = GetComponent<PhotonView>();
    }


    [PunRPC]
    internal void setStrengthRPC(string steamID, float newStrength)
    {
        PlayerAvatar avatar = SemiFunc.PlayerAvatarGetFromSteamID(steamID);
        if ((bool)avatar)
        {
            avatar.physGrabber.grabStrength = newStrength;
        }
    }
}

public class HealthManager : MonoBehaviour
{
    internal PhotonView photonView;

    private void Start()
    {
        photonView = GetComponent<PhotonView>();
    }


    [PunRPC]
    internal void setHealthRPC(string steamID, int maxHealth, int health)
    {
        PlayerAvatar avatar = SemiFunc.PlayerAvatarGetFromSteamID(steamID);
        if ((bool)avatar)
        {
            avatar.playerHealth.maxHealth = maxHealth;
            avatar.playerHealth.health = health;
        }
    }
}


public class Upgrader : MonoBehaviour
{
    private ItemToggle _itemToggle;
    private void Start() => _itemToggle = GetComponent<ItemToggle>();
    public static void UpdateStat(int amount, string steamId, string stat)
    {
        var dict =
            StatsManager.instance.dictionaryOfDictionaries[stat]
            as Dictionary<string, int>;

        if (!dict.ContainsKey(steamId))
            dict[steamId] = 0;

        dict[steamId] += amount;
    }


    public static int GetStat(string steamId, string upgradeName)
    {
        var dict =
            StatsManager.instance.dictionaryOfDictionaries[upgradeName]
            as Dictionary<string, int>;

        return dict.TryGetValue(steamId, out var v) ? v : 0;
    }

    public void upgrade()
    {
        string steamID = SemiFunc.PlayerGetSteamID(SemiFunc.PlayerAvatarGetFromPhotonID(_itemToggle.playerTogglePhotonID));
        Debug.Log("Your Mana Regen before: " + GetStat(steamID, "playerUpgradeManaRegeneration"));
        UpdateStat(1, steamID, "playerUpgradeManaRegeneration");
        RepoRoles.Update_ManaRegeneration();
        Debug.Log("Your Mana Regen after: " + GetStat(steamID, "playerUpgradeManaRegeneration"));
    }

    public void upgradeScoutCooldown()
    {
        string steamID = SemiFunc.PlayerGetSteamID(SemiFunc.PlayerAvatarGetFromPhotonID(_itemToggle.playerTogglePhotonID));
        Debug.Log("Your Scout Cooldown Upgrades before: " + GetStat(steamID, "playerUpgradeScoutCooldownReduction"));
        UpdateStat(1, steamID, "playerUpgradeScoutCooldownReduction");
        RepoRoles.Update_ScoutCooldown();
        Debug.Log("Your Scout Cooldown Upgrades after: " + GetStat(steamID, "playerUpgradeScoutCooldownReduction"));
    }
}


[HarmonyPatch(typeof(StatsManager), "Start")]
static class StatsManagerPatch
{
    static void Prefix(StatsManager __instance)
    {
        // register our two dictionaries just like MoreUpgrades does
        __instance.dictionaryOfDictionaries.Add(
            "playerUpgradeManaRegeneration",
            new Dictionary<string, int>());

        __instance.dictionaryOfDictionaries.Add(
            "playerUpgradeScoutCooldownReduction",
            new Dictionary<string, int>());
    }
}

public class ManaUI : SemiUI
{
    public TextMeshProUGUI Text;
    public static ManaUI instance;
    public TextMeshProUGUI textManaMax;

    public override void Start()
    {
        base.Start();
        ManaUI.instance = this;

        Text = base.GetComponent<TextMeshProUGUI>();
        Text.text = "0" + Mathf.Ceil(guiManager.aviableMana).ToString();
    }

    public void SetMana(float mana, float maxMana)
    {
        Text = base.GetComponent<TextMeshProUGUI>();
        textManaMax = base.transform.Find("EnergyMax").GetComponent<TextMeshProUGUI>();

        if (ManaHelper.val == null)
            ManaHelper.val = GameObject.Find("Energy");

        if (Text == null || textManaMax == null) return;
        Text.text = "0" + Mathf.Ceil(mana).ToString();
        textManaMax.text = "<b>/</b>" + "0" + Mathf.Ceil(maxMana).ToString();
    }
}

public class ManaHelper : MonoBehaviour
{
    public static GameObject val;
    public static GameObject val2;
    public static ManaUI manaUI;
    public static void CreateUI()
    {
        val = GameObject.Find("Energy");
        val2 = Object.Instantiate<GameObject>(val, val.transform.parent);

        val2.name = "Mana";
        Transform transform = val2.transform;
        transform.localPosition -= new Vector3(0f, 33f, 0f);
        EnergyUI component = val2.GetComponent<EnergyUI>();

        if (component != null)
        {
            Object.DestroyImmediate(component);
        }

        TextMeshProUGUI[] componentsInChildren = val2.GetComponentsInChildren<TextMeshProUGUI>();
        TextMeshProUGUI[] array = componentsInChildren;

        foreach (TextMeshProUGUI val3 in array)
        {
            val3.color = new Color(0f, 0.384f, 1f);
        }


        Texture2D texture = guiManager.manaTexture;
        Sprite newSprite = Sprite.Create(
            texture,
            new Rect(0, 0, texture.width, texture.height),
            new Vector2(0.5f, 0.5f)
        );

        val2.transform.gameObject.GetComponentInChildren<Image>().sprite = newSprite;
        val2.transform.gameObject.GetComponentInChildren<Image>().color = Color.white;

        manaUI = val2.AddComponent<ManaUI>();
        manaUI.SetMana(8f, 8f);
    }
}

[HarmonyPatch(typeof(TruckScreenText))]
static class Sender
{
    private static bool hasRanOnce;
    public static ClassManager manager = new ClassManager();

    public static readonly Harmony harmonyPatcher = new Harmony("sender.reporoles.mod");

    [HarmonyPatch("ArrowPointAtGoalLogic")]
    [HarmonyPrefix]
    static void PrefixMethod()
    {
        if (hasRanOnce)
            return;
        manager.assignRoleFromConfig(PlayerController.instance);
        BepInEx.Logging.Logger.CreateLogSource("ChatMsgMgr").LogInfo("Successfully rolled role!");
        hasRanOnce = true;
    }

    [HarmonyPatch("Start")]
    [HarmonyPrefix]
    static void StartPrefix()
    {
        hasRanOnce = false;
    }
}

[HarmonyPatch(typeof(PlayerAvatar))]
static class PlayerAvatarPatch
{
    [HarmonyPostfix, HarmonyPatch(nameof(PlayerAvatar.Start))]
    private static void Start_Postfix(PlayerAvatar __instance)
    {
        if (__instance.GetComponent<ReaperManager>() == null)
        {
            __instance.AddComponent<ReaperManager>();
        }
    }
}

public class ScoutMarker : MonoBehaviour
{
    private int cooldownTicker = 0; // 1500
    private int activeTicker = 250;
    private bool isActive = false;
    private bool onCooldown = true;
    public static int reductionUpgrades;

    private void OnGUI()
    {
        if (!SemiFunc.RunIsLevel() || SemiFunc.RunIsShop() || PlayerAvatar.instance.playerHealth.health <= 0)
            return;

        GUIStyle timerStyle = new GUIStyle(GUI.skin.label)
        {
            alignment = TextAnchor.MiddleCenter,
            fontSize = 22
        };
        timerStyle.normal.textColor = new Color(0.902f, 0.733f, 0.11f);

        if (guiManager.customFont != null)
            timerStyle.font = guiManager.customFont;

        if (EnemyDirector.instance.enemiesSpawned != null && ClassManager.isScout && isActive)
        {
            GUIStyle labelStyle = new GUIStyle(GUI.skin.label)
            {
                alignment = TextAnchor.MiddleCenter,
                fontSize = 22
            };
            labelStyle.normal.textColor = Color.red;

            if (guiManager.customFont != null)
                labelStyle.font = guiManager.customFont;

            foreach (EnemyParent enemyParent in EnemyDirector.instance.enemiesSpawned)
            {
                if (enemyParent == null || !enemyParent.EnableObject.activeInHierarchy) { continue; }

                Enemy enemy = (Enemy)AccessTools.Field(typeof(EnemyParent), "Enemy").GetValue(enemyParent);
                if (enemy != null && Camera.main != null)
                {
                    Vector3 screenPosition = Camera.main.WorldToViewportPoint(enemy.CenterTransform.position);
                    if (screenPosition.z < 0) { continue; }

                    string enemyName = enemyParent.enemyName;
                    string name = $"{enemyName} [{screenPosition.z:F0}m]";
                    Vector2 textSize = labelStyle.CalcSize(new GUIContent(name));

                    float x = screenPosition.x * Screen.width - textSize.x / 2;
                    float y = Screen.height - screenPosition.y * Screen.height - textSize.y / 2;

                    GUI.Label(new Rect(x, y, textSize.x, textSize.y), name, labelStyle);
                }
            }

            if (isActive && !onCooldown)
            {
                int xTimer = Screen.width / 2 - 100;
                int yTimer = Screen.height - 125;
                string timer = (activeTicker / 50).ToString();

                string timerText = "Ability runs out in: " + timer + "s";
                
                GUI.Label(new Rect(xTimer, yTimer, 200, 50), timerText, timerStyle);
            }
        }
        if (!isActive && ClassManager.isScout && onCooldown)
        {
            int xTimer2 = Screen.width / 2 - 100;
            int yTimer2 = Screen.height - 125;
            string timer2 = (cooldownTicker / 50).ToString();
            timerStyle.normal.textColor = new Color(0.851f, 0.667f, 0.098f);

            string timerText2 = "Ability is ready in: " + timer2 + "s";

            GUI.Label(new Rect(xTimer2, yTimer2, 200, 60), timerText2, timerStyle);
        }
        else if (!isActive && ClassManager.isScout && cooldownTicker <= 0)
        {
            int xTimer2 = Screen.width / 2 - 100;
            int yTimer2 = Screen.height - 125;
            string timer2 = (cooldownTicker / 50).ToString();
            timerStyle.normal.textColor = Color.green;

            string timerText2 = "Ability is ready!\nPress [" + RepoRoles.scoutKey.Value + "] to activate";

            GUI.Label(new Rect(xTimer2, yTimer2, 200, 60), timerText2, timerStyle);
        }
    }

    private void FixedUpdate()
    {
        if (ClassManager.isScout)
        {
            if (isActive)
            {
                activeTicker--;
            }

            if (activeTicker <= 0)
            {
                isActive = false;
                activeTicker = 250;
                onCooldown = true;

                if (reductionUpgrades <= 5)
                    cooldownTicker = 2000 - (reductionUpgrades * 250);
                else
                {
                    cooldownTicker = 750;
                }
            }

            if (cooldownTicker <= 0)
            {
                onCooldown = false;
            }

            if (cooldownTicker >= 0 && onCooldown == true)
            {
                cooldownTicker--;
            }
        }
    }

    private void Update()
    {
        if (ClassManager.isScout && SemiFunc.RunIsLevel() && !SemiFunc.RunIsShop() && !ChatManager.instance.chatActive)
        {
            if (onCooldown == false)
            {
                if (UnityEngine.Input.GetKeyDown(RepoRoles.scoutKey.Value))
                {
                    isActive = true;
                }
            }
        }
    }
}

[HarmonyPatch(typeof(EnemyHealth))]
static class ReaperPatch
{
    public static readonly Harmony harmonyPatcher = new Harmony("sender.reporoles.mod");
    private static ReaperManager rMan;

    [HarmonyPatch("Death")]
    [HarmonyPrefix]
    public static void PrefixMethod()
    {
        if (SemiFunc.PlayerAvatarGetFromSteamID(PlayerController.instance.playerSteamID).GetComponent<ReaperManager>() != null)
        {
            rMan = SemiFunc.PlayerAvatarGetFromSteamID(PlayerController.instance.playerSteamID).GetComponent<ReaperManager>();
        }
        else
        {
            Debug.LogError("Failed to get Reaper Manager! Please contact the mod developer about this.");
        }

        if (rMan.isReaper && rMan != null)
        {
            rMan.kills += 1;
        }
        else
        {
            Debug.LogError("Unable to find ReaperManager in PlayerAvatar.instance. Please report this to the mod author.");
        }
    }
}

public class ReaperManager : MonoBehaviour
{
    public bool isReaper;
    public int kills;

    private int enemyDeathTimer = 0;

    internal PhotonView photonView;

    private void Start()
    {
        photonView = GetComponent<PhotonView>();
    }

    [PunRPC]
    internal void setReaperStatusRPC(string steamID, bool isReaper)
    {
        PlayerAvatar avatar = SemiFunc.PlayerAvatarGetFromSteamID(steamID);
        if ((bool)avatar)
        {
            avatar.GetComponent<ReaperManager>().isReaper = isReaper;
        }
    }

    private void Update()
    {
        foreach (PlayerAvatar avatar in SemiFunc.PlayerGetAll())
        {
            if (avatar.GetComponent<ReaperManager>() != null && avatar.GetComponent<ReaperManager>().isReaper && (int)AccessTools.Field(typeof(PlayerHealth), "health").GetValue(avatar.playerHealth) > 0)
            {
                if (!SemiFunc.IsMultiplayer())
                {
                    if (SemiFunc.PlayerAvatarGetFromSteamID(PlayerController.instance.playerSteamID).GetComponent<ReaperManager>().kills > 0 && enemyDeathTimer <= 0)
                    {
                        AccessTools.Field(typeof(PlayerHealth), "maxHealth").SetValue(avatar.playerHealth, (int)AccessTools.Field(typeof(PlayerHealth), "maxHealth").GetValue(avatar.playerHealth) + 5);
                        if ((int)AccessTools.Field(typeof(PlayerHealth), "health").GetValue(avatar.playerHealth) + 30 > (int)AccessTools.Field(typeof(PlayerHealth), "maxHealth").GetValue(avatar.playerHealth))
                        {
                            avatar.playerHealth.Heal((int)AccessTools.Field(typeof(PlayerHealth), "maxHealth").GetValue(avatar.playerHealth) - (int)AccessTools.Field(typeof(PlayerHealth), "health").GetValue(avatar.playerHealth));
                        }
                        else
                        {
                            avatar.playerHealth.Heal(30);
                        }

                        avatar.GetComponent<ReaperManager>().kills = 0;
                        enemyDeathTimer = 50;
                    }
                }
                else if (avatar.photonView != null)
                {
                    avatar.photonView.RPC("giveReaperStatsRPC", RpcTarget.All, avatar.steamID);
                }
            }
        }
        

        if (enemyDeathTimer > 0)
        {
            enemyDeathTimer -= 1;
        }
    }

    [PunRPC]
    internal void giveReaperStatsRPC(string steamID)
    {
        PlayerAvatar avatar = SemiFunc.PlayerAvatarGetFromSteamID(steamID);
        if ((bool)avatar)
        {
            if (SemiFunc.PlayerAvatarGetFromSteamID(PlayerController.instance.playerSteamID).GetComponent<ReaperManager>().kills > 0 && enemyDeathTimer <= 0)
            {
                AccessTools.Field(typeof(PlayerHealth), "maxHealth").SetValue(avatar.playerHealth, (int)AccessTools.Field(typeof(PlayerHealth), "maxHealth").GetValue(avatar.playerHealth) + 5);
                if ((int)AccessTools.Field(typeof(PlayerHealth), "health").GetValue(avatar.playerHealth) + 30 > (int)AccessTools.Field(typeof(PlayerHealth), "maxHealth").GetValue(avatar.playerHealth))
                {
                    avatar.playerHealth.Heal((int)AccessTools.Field(typeof(PlayerHealth), "maxHealth").GetValue(avatar.playerHealth) - (int)AccessTools.Field(typeof(PlayerHealth), "health").GetValue(avatar.playerHealth));
                }
                else
                {
                    avatar.playerHealth.Heal(30);
                }

                avatar.GetComponent<ReaperManager>().kills = 0;
                enemyDeathTimer = 50;
            }
        }
    }
}