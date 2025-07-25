using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.Versioning;
using System.Security;
using System.Security.Permissions;
using BepInEx;
using BepInEx.Configuration;
using BepInEx.Logging;
using Bobi;
using HarmonyLib;
using MenuLib;
using MenuLib.MonoBehaviors;
using Microsoft.CodeAnalysis;
using Photon.Pun;
using R.E.P.O.Roles;
using R.E.P.O.Roles.patches;
using REPOLib;
using REPOLib.Modules;
using TMPro;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

[assembly: CompilationRelaxations(8)]
[assembly: RuntimeCompatibility(WrapNonExceptionThrows = true)]
[assembly: Debuggable(DebuggableAttribute.DebuggingModes.Default | DebuggableAttribute.DebuggingModes.DisableOptimizations | DebuggableAttribute.DebuggingModes.IgnoreSymbolStoreSequencePoints | DebuggableAttribute.DebuggingModes.EnableEditAndContinue)]
[assembly: TargetFramework(".NETStandard,Version=v2.1", FrameworkDisplayName = ".NET Standard 2.1")]
[assembly: IgnoresAccessChecksTo("")]
[assembly: AssemblyCompany("KingRobin")]
[assembly: AssemblyConfiguration("Debug")]
[assembly: AssemblyFileVersion("1.0.0.0")]
[assembly: AssemblyInformationalVersion("1.0.0")]
[assembly: AssemblyProduct("REPO Roles (SDK)")]
[assembly: AssemblyTitle("kingrobin.repo_roles")]
[assembly: SecurityPermission(SecurityAction.RequestMinimum, SkipVerification = true)]
[assembly: AssemblyVersion("1.0.0.0")]
[module: UnverifiableCode]
[module: RefSafetyRules(11)]
namespace Microsoft.CodeAnalysis
{
	[CompilerGenerated]
	[Microsoft.CodeAnalysis.Embedded]
	internal sealed class EmbeddedAttribute : Attribute
	{
	}
}
namespace System.Runtime.CompilerServices
{
	[CompilerGenerated]
	[Microsoft.CodeAnalysis.Embedded]
	[AttributeUsage(AttributeTargets.Class | AttributeTargets.Property | AttributeTargets.Field | AttributeTargets.Event | AttributeTargets.Parameter | AttributeTargets.ReturnValue | AttributeTargets.GenericParameter, AllowMultiple = false, Inherited = false)]
	internal sealed class NullableAttribute : Attribute
	{
		public readonly byte[] NullableFlags;

		public NullableAttribute(byte P_0)
		{
			NullableFlags = new byte[1] { P_0 };
		}

		public NullableAttribute(byte[] P_0)
		{
			NullableFlags = P_0;
		}
	}
	[CompilerGenerated]
	[Microsoft.CodeAnalysis.Embedded]
	[AttributeUsage(AttributeTargets.Class | AttributeTargets.Struct | AttributeTargets.Method | AttributeTargets.Interface | AttributeTargets.Delegate, AllowMultiple = false, Inherited = false)]
	internal sealed class NullableContextAttribute : Attribute
	{
		public readonly byte Flag;

		public NullableContextAttribute(byte P_0)
		{
			Flag = P_0;
		}
	}
	[CompilerGenerated]
	[Microsoft.CodeAnalysis.Embedded]
	[AttributeUsage(AttributeTargets.Module, AllowMultiple = false, Inherited = false)]
	internal sealed class RefSafetyRulesAttribute : Attribute
	{
		public readonly int Version;

		public RefSafetyRulesAttribute(int P_0)
		{
			Version = P_0;
		}
	}
}
public class ScoutMarker : MonoBehaviour
{
	private int cooldownTicker = 0;

	private int activeTicker = 250;

	private bool isActive = false;

	private bool onCooldown = true;

	public static int reductionUpgrades;

	private void OnGUI()
	{
		if (!SemiFunc.RunIsLevel() || SemiFunc.RunIsShop() || PlayerAvatar.instance.playerHealth.health <= 0)
		{
			return;
		}
		GUIStyle val = new GUIStyle(GUI.skin.label)
		{
			alignment = (TextAnchor)4,
			fontSize = 22
		};
		val.normal.textColor = new Color(0.902f, 0.733f, 0.11f);
		if ((Object)(object)guiManager.customFont != (Object)null)
		{
			val.font = guiManager.customFont;
		}
		if (EnemyDirector.instance.enemiesSpawned != null && ClassManager.isScout && isActive)
		{
			GUIStyle val2 = new GUIStyle(GUI.skin.label)
			{
				alignment = (TextAnchor)4,
				fontSize = 22
			};
			val2.normal.textColor = Color.red;
			if ((Object)(object)guiManager.customFont != (Object)null)
			{
				val2.font = guiManager.customFont;
			}
			foreach (EnemyParent item in EnemyDirector.instance.enemiesSpawned)
			{
				if ((Object)(object)item == (Object)null || !item.EnableObject.activeInHierarchy)
				{
					continue;
				}
				Enemy val3 = (Enemy)AccessTools.Field(typeof(EnemyParent), "Enemy").GetValue(item);
				if ((Object)(object)val3 != (Object)null && (Object)(object)Camera.main != (Object)null)
				{
					Vector3 val4 = Camera.main.WorldToViewportPoint(val3.CenterTransform.position);
					if (!(val4.z < 0f))
					{
						string enemyName = item.enemyName;
						string text = $"{enemyName} [{val4.z:F0}m]";
						Vector2 val5 = val2.CalcSize(new GUIContent(text));
						float num = val4.x * (float)Screen.width - val5.x / 2f;
						float num2 = (float)Screen.height - val4.y * (float)Screen.height - val5.y / 2f;
						GUI.Label(new Rect(num, num2, val5.x, val5.y), text, val2);
					}
				}
			}
			if (isActive && !onCooldown)
			{
				int num3 = Screen.width / 2 - 100;
				int num4 = Screen.height - 125;
				string text2 = (activeTicker / 50).ToString();
				string text3 = "Ability runs out in: " + text2 + "s";
				GUI.Label(new Rect((float)num3, (float)num4, 200f, 50f), text3, val);
			}
		}
		if (!isActive && ClassManager.isScout && onCooldown)
		{
			int num5 = Screen.width / 2 - 100;
			int num6 = Screen.height - 125;
			string text4 = (cooldownTicker / 50).ToString();
			val.normal.textColor = new Color(0.851f, 0.667f, 0.098f);
			string text5 = "Ability is ready in: " + text4 + "s";
			GUI.Label(new Rect((float)num5, (float)num6, 200f, 60f), text5, val);
		}
		else if (!isActive && ClassManager.isScout && cooldownTicker <= 0)
		{
			int num7 = Screen.width / 2 - 100;
			int num8 = Screen.height - 125;
			string text6 = (cooldownTicker / 50).ToString();
			val.normal.textColor = Color.green;
			KeyCode value = RepoRoles.scoutKey.Value;
			string text7 = "Ability is ready!\nPress [" + ((object)(KeyCode)(ref value)).ToString() + "] to activate";
			GUI.Label(new Rect((float)num7, (float)num8, 200f, 60f), text7, val);
		}
	}

	private void FixedUpdate()
	{
		if (!ClassManager.isScout)
		{
			return;
		}
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
			{
				cooldownTicker = 2000 - reductionUpgrades * 250;
			}
			else
			{
				cooldownTicker = 750;
			}
		}
		if (cooldownTicker <= 0)
		{
			onCooldown = false;
		}
		if (cooldownTicker >= 0 && onCooldown)
		{
			cooldownTicker--;
		}
	}

	private void Update()
	{
		if (ClassManager.isScout && SemiFunc.RunIsLevel() && !SemiFunc.RunIsShop() && !ChatManager.instance.chatActive && !onCooldown && Input.GetKeyDown(RepoRoles.scoutKey.Value))
		{
			isActive = true;
		}
	}
}
namespace Bobi
{
	[BepInPlugin("reporoles.mod", "RepoRoles", "2.0.6")]
	public class RepoRoles : BaseUnityPlugin
	{
		public const string pluginGuid = "reporoles.mod";

		public const string pluginName = "RepoRoles";

		public const string pluginVersion = "2.0.6";

		public readonly Harmony harmony = new Harmony("reporoles.mod");

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

		public ConfigDefinition mageTopManaDef = new ConfigDefinition("Role", "Place where mana is shown");

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

		private REPOSlider slider;

		private REPOPopupPage configPage;

		private REPOButton buttonOpen;

		private REPOButton buttonOpenLobby;

		private REPOButton buttonClose;

		private REPOToggle afterDeathRoleToggle;

		private REPOToggle mageTopMana;

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
			return Path.Combine(Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location), "REPORoles_assets2");
		}

		private void Awake()
		{
			mls = Logger.CreateLogSource("reporoles.mod");
			savedRole = ((BaseUnityPlugin)this).Config.Bind<string>(selectedRoleDef, "Random", (ConfigDescription)null);
			assignRoleAfterRevive = ((BaseUnityPlugin)this).Config.Bind<bool>(afterDeathRoleDef, false, (ConfigDescription)null);
			mageTopManaConf = ((BaseUnityPlugin)this).Config.Bind<bool>(mageTopManaDef, true, (ConfigDescription)null);
			mageTopManaBool = mageTopManaConf.Value;
			showGUIAtStart = ((BaseUnityPlugin)this).Config.Bind<bool>(showGUIAtStartDef, true, (ConfigDescription)null);
			enableRunner = ((BaseUnityPlugin)this).Config.Bind<bool>(enableRunnerDef, true, (ConfigDescription)null);
			enableTank = ((BaseUnityPlugin)this).Config.Bind<bool>(enableTankDef, true, (ConfigDescription)null);
			enableGambler = ((BaseUnityPlugin)this).Config.Bind<bool>(enableGamblerDef, true, (ConfigDescription)null);
			enableStrongman = ((BaseUnityPlugin)this).Config.Bind<bool>(enableStrongmanDef, true, (ConfigDescription)null);
			enableRL = ((BaseUnityPlugin)this).Config.Bind<bool>(enableRLDef, true, (ConfigDescription)null);
			enableAthletic = ((BaseUnityPlugin)this).Config.Bind<bool>(enableAthleticDef, true, (ConfigDescription)null);
			enableMage = ((BaseUnityPlugin)this).Config.Bind<bool>(enableMageDef, true, (ConfigDescription)null);
			enableReaper = ((BaseUnityPlugin)this).Config.Bind<bool>(enableReaperDef, true, (ConfigDescription)null);
			enableScout = ((BaseUnityPlugin)this).Config.Bind<bool>(enableScoutDef, true, (ConfigDescription)null);
			MenuAPI.AddElementToSettingsMenu((BuilderDelegate)delegate(Transform parent)
			{
				if ((Object)(object)configPage == (Object)null)
				{
					configPage = MenuAPI.CreateREPOPopupPage("REPO Roles", (PresetSide)0, true, true, 1.5f);
				}
				buttonOpen = MenuAPI.CreateREPOButton("REPO Roles Config", (Action)delegate
				{
					configPage.OpenPage(false);
				}, parent, new Vector2(500f, 10f));
				configPage.AddElementToScrollView((ScrollViewBuilderDelegate)delegate(Transform scrollView)
				{
					//IL_0099: Unknown result type (might be due to invalid IL or missing references)
					if ((Object)(object)slider == (Object)null)
					{
						slider = MenuAPI.CreateREPOSlider("REPO Roles", "Choose your role", (Action<string>)delegate(string s)
						{
							sliderConf(s);
						}, scrollView, new string[10] { "Random", "Runner", "Tank", "Gambler", "Strongman", "Ranged Looter", "Athletic", "Mage", "Reaper", "Scout" }, savedRole.Value, new Vector2(0f, 0f), "", "", (BarBehavior)0);
					}
					return ((REPOElement)slider).rectTransform;
				}, 0f, 0f);
				configPage.AddElementToScrollView((ScrollViewBuilderDelegate)delegate(Transform scrollView)
				{
					if ((Object)(object)afterDeathRoleToggle == (Object)null)
					{
						afterDeathRoleToggle = MenuAPI.CreateREPOToggle("Reassign role on respawn", (Action<bool>)delegate(bool b)
						{
							aDRC(b);
						}, scrollView, new Vector2(0f, 0f), "ON", "OFF", assignRoleAfterRevive.Value);
					}
					return ((REPOElement)afterDeathRoleToggle).rectTransform;
				}, 0f, 0f);
				configPage.AddElementToScrollView((ScrollViewBuilderDelegate)delegate(Transform scrollView)
				{
					if ((Object)(object)mageTopMana == (Object)null)
					{
						mageTopMana = MenuAPI.CreateREPOToggle("Mage Mana Position", (Action<bool>)delegate(bool b)
						{
							mageManaFunc(b);
						}, scrollView, new Vector2(0f, 0f), "TOP", "LEFT", mageTopManaConf.Value);
					}
					return ((REPOElement)mageTopMana).rectTransform;
				}, 0f, 0f);
				configPage.AddElement((BuilderDelegate)delegate(Transform parent)
				{
					if ((Object)(object)buttonClose == (Object)null)
					{
						buttonClose = MenuAPI.CreateREPOButton("Back", (Action)delegate
						{
							configPage.ClosePage(false);
						}, parent, Vector2.zero);
					}
				});
			});
			MenuAPI.AddElementToLobbyMenu((BuilderDelegate)delegate(Transform parent)
			{
				//IL_001e: Unknown result type (might be due to invalid IL or missing references)
				buttonOpenLobby = MenuAPI.CreateREPOButton("REPO Roles Config", (Action)delegate
				{
					configPage.OpenPage(false);
				}, parent, new Vector2(500f, 500f));
			});
			if ((Object)(object)Instance == (Object)null)
			{
				Instance = this;
			}
			harmony.PatchAll(typeof(RepoRoles));
			harmony.PatchAll(typeof(Sender));
			mls.LogInfo((object)"Init");
			SceneManager.sceneLoaded += delegate
			{
				if (!Object.op_Implicit((Object)(object)GUIinstance))
				{
					GameObject val3 = new GameObject();
					GUIinstance = val3.AddComponent<guiManager>();
					GUIinstance.text = "";
					GUIinstance.color = Color.cyan;
					Object.DontDestroyOnLoad((Object)(object)val3);
				}
			};
			toggleKey = ((BaseUnityPlugin)this).Config.Bind<KeyCode>(def, (KeyCode)114, (ConfigDescription)null);
			showSpellsKey = ((BaseUnityPlugin)this).Config.Bind<KeyCode>(showSpellsDef, (KeyCode)109, (ConfigDescription)null);
			healKey = ((BaseUnityPlugin)this).Config.Bind<KeyCode>(healDef, (KeyCode)104, (ConfigDescription)null);
			speedKey = ((BaseUnityPlugin)this).Config.Bind<KeyCode>(speedDef, (KeyCode)106, (ConfigDescription)null);
			overchargeKey = ((BaseUnityPlugin)this).Config.Bind<KeyCode>(overchargeDef, (KeyCode)111, (ConfigDescription)null);
			jumpKey = ((BaseUnityPlugin)this).Config.Bind<KeyCode>(jumpDef, (KeyCode)107, (ConfigDescription)null);
			staminaKey = ((BaseUnityPlugin)this).Config.Bind<KeyCode>(staminaDef, (KeyCode)110, (ConfigDescription)null);
			customRoleNameRunner = ((BaseUnityPlugin)this).Config.Bind<string>(customRoleNameRunnerDef, "Runner", (ConfigDescription)null);
			customRoleNameTank = ((BaseUnityPlugin)this).Config.Bind<string>(customRoleNameTankDef, "Tank", (ConfigDescription)null);
			customRoleNameGambler = ((BaseUnityPlugin)this).Config.Bind<string>(customRoleNameGamblerDef, "Gambler", (ConfigDescription)null);
			customRoleNameStrongman = ((BaseUnityPlugin)this).Config.Bind<string>(customRoleNameStrongmanDef, "Strongman", (ConfigDescription)null);
			customRoleNameRL = ((BaseUnityPlugin)this).Config.Bind<string>(customRoleNameRLDef, "Ranged Looter", (ConfigDescription)null);
			customRoleNameAthletic = ((BaseUnityPlugin)this).Config.Bind<string>(customRoleNameAthleticDef, "Athletic", (ConfigDescription)null);
			customRoleNameMage = ((BaseUnityPlugin)this).Config.Bind<string>(customRoleNameMageDef, "Mage", (ConfigDescription)null);
			customRoleNameReaper = ((BaseUnityPlugin)this).Config.Bind<string>(customRoleNameReaperDef, "Reaper", (ConfigDescription)null);
			customRoleNameScout = ((BaseUnityPlugin)this).Config.Bind<string>(customRoleNameScoutDef, "Scout", (ConfigDescription)null);
			customRoleDecRunner = ((BaseUnityPlugin)this).Config.Bind<string>(customRoleDesRunnerDef, "You have more stamina and run much faster than everyone else!", (ConfigDescription)null);
			customRoleDecTank = ((BaseUnityPlugin)this).Config.Bind<string>(customRoleDesTankDef, "You walk slower but your hp is doubled!", (ConfigDescription)null);
			customRoleDecGambler = ((BaseUnityPlugin)this).Config.Bind<string>(customRoleDesGamblerDef, "You rolled random effects:", (ConfigDescription)null);
			customRoleDecStrongman = ((BaseUnityPlugin)this).Config.Bind<string>(customRoleDesStrongmanDef, "You\u00b4re incredibly strong!", (ConfigDescription)null);
			customRoleDecRL = ((BaseUnityPlugin)this).Config.Bind<string>(customRoleDesRLDef, "You can reach objects from far away!", (ConfigDescription)null);
			customRoleDecAthletic = ((BaseUnityPlugin)this).Config.Bind<string>(customRoleDesAthleticDef, "You have more stamina, strength and can jump higher", (ConfigDescription)null);
			customRoleDecMage = ((BaseUnityPlugin)this).Config.Bind<string>(customRoleDesMageDef, "You are able to use your mana to become incredibly strong!", (ConfigDescription)null);
			customRoleDecReaper = ((BaseUnityPlugin)this).Config.Bind<string>(customRoleDesReaperDef, "For each enemy you and your friends kill, you become stronger!", (ConfigDescription)null);
			customRoleDecScout = ((BaseUnityPlugin)this).Config.Bind<string>(customRoleDesScoutDef, "Your stamina is more efficient and by pressing [G] you can see all enemies around you.", (ConfigDescription)null);
			scoutKey = ((BaseUnityPlugin)this).Config.Bind<KeyCode>(scoutButtonDef, (KeyCode)103, (ConfigDescription)null);
			harmony.PatchAll(typeof(PunManagerPatch));
			harmony.PatchAll(typeof(PlayerAvatarPatch));
			harmony.PatchAll(typeof(StatsManagerPatch));
			BundleLoader.LoadBundle(getPath(), (Action<AssetBundle>)delegate(AssetBundle assetBundle)
			{
				Item val = assetBundle.LoadAsset<Item>("Mana Regeneration Upgrade");
				Items.RegisterItem(val);
				Item val2 = assetBundle.LoadAsset<Item>("Scout Cooldown Upgrade");
				Items.RegisterItem(val2);
			}, false);
		}

		public static void Update_ManaRegeneration()
		{
			if (LevelGenerator.Instance.Generated && !SemiFunc.MenuLevel())
			{
				int stat = Upgrader.GetStat(PlayerAvatar.instance.steamID, "playerUpgradeManaRegeneration");
				guiManager.manaRegenRate = Upgrader.GetStat(PlayerAvatar.instance.steamID, "playerUpgradeManaRegeneration");
			}
		}

		public static void Update_ScoutCooldown()
		{
			if (LevelGenerator.Instance.Generated && !SemiFunc.MenuLevel())
			{
				int stat = Upgrader.GetStat(PlayerAvatar.instance.steamID, "playerUpgradeScoutCooldownReduction");
				ScoutMarker.reductionUpgrades = Upgrader.GetStat(PlayerAvatar.instance.steamID, "playerUpgradeScoutCooldownReduction");
			}
		}
	}
	[HarmonyPatch(typeof(PunManager))]
	internal static class PunManagerPatch
	{
		[HarmonyPostfix]
		[HarmonyPatch("Start")]
		private static void Start_Postfix(PunManager __instance)
		{
			if ((Object)(object)((Component)__instance).GetComponent<StrengthManager>() == (Object)null)
			{
				ComponentHolderProtocol.AddComponent<StrengthManager>((Object)(object)__instance);
			}
			if ((Object)(object)((Component)__instance).GetComponent<HealthManager>() == (Object)null)
			{
				ComponentHolderProtocol.AddComponent<HealthManager>((Object)(object)__instance);
			}
			if ((Object)(object)((Component)__instance).GetComponent<ScoutMarker>() == (Object)null)
			{
				ComponentHolderProtocol.AddComponent<ScoutMarker>((Object)(object)__instance);
			}
		}
	}
}
namespace R.E.P.O.Roles
{
	public class ClassManager
	{
		private Random rnd = new Random();

		public int chosenRoleId;

		private StrengthManager strManager = new StrengthManager();

		public string chosenRole;

		public int roleAmount = 9;

		internal static ManualLogSource mls;

		public RepoRoles repoRoles = new RepoRoles();

		public ReaperManager rMan;

		public readonly Harmony harmonyPatcher = new Harmony("patches.reporoles.mod");

		private float origMoveSpeed;

		private float origSprintSpeed;

		private float origCrouchSpeed;

		private float speedMultiplier;

		private int origPlayerHealth;

		private int origMaxPlayerHealth;

		private float origJumpForce;

		private float origMaxEnergy;

		private float origGrabStrength;

		private float origGrabRange;

		public static int stackKills;

		public static bool isTank;

		public static bool isScout;

		private string[] roleNames = new string[10] { "Random", "Runner", "Tank", "Gambler", "Strongman", "Ranged Looter", "Athletic", "Mage", "Reaper", "Scout" };

		public ClassManager()
		{
			if (mls == null)
			{
				mls = Logger.CreateLogSource("ClassManager");
			}
			origMoveSpeed = PlayerController.instance.MoveSpeed;
			origSprintSpeed = PlayerController.instance.SprintSpeed;
			origCrouchSpeed = PlayerController.instance.CrouchSpeed;
			speedMultiplier = 1.5f;
			origJumpForce = PlayerController.instance.JumpForce;
			origPlayerHealth = (int)AccessTools.Field(typeof(PlayerHealth), "health").GetValue(PlayerAvatar.instance.playerHealth);
			origMaxPlayerHealth = (int)AccessTools.Field(typeof(PlayerHealth), "maxHealth").GetValue(PlayerAvatar.instance.playerHealth);
			origMaxEnergy = PlayerController.instance.EnergyStart;
			origGrabStrength = PhysGrabber.instance.grabStrength;
			origGrabRange = PhysGrabber.instance.grabRange;
		}

		public int genGamblerEffectNr()
		{
			return rnd.Next(0, 5);
		}

		public object[] genGamblerEffects()
		{
			string[] array = new string[5] { "You walk faster", "You have more stamina", "You have more health", "You\u00b4re stronger", "You jump higher" };
			string[] array2 = new string[5] { "you walk slower", "you have less stamina", "you have less health", "you\u00b4re weaker", "you don\u00b4t jump as high" };
			int num = genGamblerEffectNr();
			int num2 = genGamblerEffectNr();
			while (num == num2)
			{
				num = genGamblerEffectNr();
				num2 = genGamblerEffectNr();
			}
			string text = array[num];
			string text2 = array2[num2];
			string text3 = RepoRoles.customRoleDecGambler.Value + " " + text + " but " + text2 + "!";
			return new object[3] { text3, num, num2 };
		}

		public void assignRoleFromConfig(PlayerController __instance)
		{
			string value = RepoRoles.savedRole.Value;
			if (value == "" || value == "Random")
			{
				chosenRoleId = rnd.Next(1, roleAmount + 1);
				mls.LogInfo((object)"Rolling random role...");
			}
			else
			{
				chosenRoleId = Array.IndexOf(roleNames, value);
			}
			assignRole(chosenRoleId, __instance);
		}

		private void modifyStrength(string steamID, float newStrength)
		{
			if (!SemiFunc.IsMultiplayer())
			{
				PlayerAvatar.instance.physGrabber.grabStrength = newStrength;
				return;
			}
			PunManager.instance.photonView.RPC("setStrengthRPC", (RpcTarget)0, new object[2] { steamID, newStrength });
		}

		private void setHealth(string steamID, int maxHealth, int health)
		{
			if (!SemiFunc.IsMultiplayer())
			{
				PlayerAvatar.instance.playerHealth.health = health;
				PlayerAvatar.instance.playerHealth.maxHealth = maxHealth;
				return;
			}
			PunManager.instance.photonView.RPC("setHealthRPC", (RpcTarget)0, new object[3] { steamID, maxHealth, health });
		}

		public void setReaperStatus(string steamID, bool isReaper)
		{
			if (!SemiFunc.IsMultiplayer())
			{
				((Component)PlayerAvatar.instance).GetComponent<ReaperManager>().isReaper = isReaper;
			}
			else if ((Object)(object)PlayerAvatar.instance.photonView != (Object)null)
			{
				PlayerAvatar.instance.photonView.RPC("setReaperStatusRPC", (RpcTarget)0, new object[2] { steamID, isReaper });
			}
		}

		public void assignRole(int roleId, PlayerController __instance)
		{
			RepoRoles.Update_ManaRegeneration();
			RepoRoles.Update_ScoutCooldown();
			if ((Object)(object)((Component)PlayerAvatar.instance).GetComponent<ReaperManager>() != (Object)null)
			{
				rMan = ((Component)PlayerAvatar.instance).GetComponent<ReaperManager>();
			}
			else
			{
				Debug.LogError((object)"Failed to get Reaper Manager! Please contact the mod developer about this.");
			}
			string text = SemiFunc.PlayerGetName(PlayerAvatar.instance);
			string key = SemiFunc.PlayerGetSteamID(PlayerAvatar.instance);
			int num = 0;
			int num2 = 0;
			int num3 = 0;
			int num4 = 0;
			int num5 = 0;
			if (StatsManager.instance.playerUpgradeHealth.TryGetValue(key, out var value))
			{
				num = value;
			}
			if (StatsManager.instance.playerUpgradeSpeed.TryGetValue(key, out var value2))
			{
				num2 = value2;
			}
			if (StatsManager.instance.playerUpgradeStamina.TryGetValue(key, out var value3))
			{
				num3 = value3;
			}
			if (StatsManager.instance.playerUpgradeStrength.TryGetValue(key, out var value4))
			{
				num4 = value4;
			}
			if (StatsManager.instance.playerUpgradeRange.TryGetValue(key, out var value5))
			{
				num5 = value5;
			}
			stackKills = 0;
			__instance.CrouchSpeed = origCrouchSpeed;
			__instance.MoveSpeed = origMoveSpeed;
			__instance.SprintSpeed = origSprintSpeed + (float)num2 * 1f;
			__instance.JumpForce = origJumpForce;
			__instance.EnergyStart = origMaxEnergy + (float)num3 * 10f;
			__instance.EnergyCurrent = origMaxEnergy + (float)num3 * 10f;
			PlayerAvatar.instance.physGrabber.grabRange = origGrabRange + 1f * (float)num5;
			modifyStrength(PlayerController.instance.playerSteamID, origGrabStrength + (float)num4 * 0.2f);
			int num6 = origMaxPlayerHealth + num * 20;
			int health = (int)AccessTools.Field(typeof(PlayerHealth), "health").GetValue(PlayerAvatar.instance.playerHealth);
			if ((int)AccessTools.Field(typeof(PlayerHealth), "health").GetValue(PlayerAvatar.instance.playerHealth) > num6)
			{
				setHealth(PlayerController.instance.playerSteamID, num6, num6);
			}
			else
			{
				setHealth(PlayerController.instance.playerSteamID, num6, health);
			}
			guiManager.isMage = false;
			isTank = false;
			setReaperStatus(PlayerController.instance.playerSteamID, isReaper: false);
			isScout = false;
			if (!SemiFunc.RunIsLevel() || SemiFunc.RunIsShop())
			{
				return;
			}
			if (!RepoRoles.enableRunner.Value && !RepoRoles.enableTank.Value && !RepoRoles.enableGambler.Value && !RepoRoles.enableStrongman.Value && !RepoRoles.enableRL.Value && !RepoRoles.enableAthletic.Value && !RepoRoles.enableMage.Value && !RepoRoles.enableReaper.Value && !RepoRoles.enableScout.Value)
			{
				Debug.LogError((object)"WARNING! You disabled all roles in the config file. You will not get any roles until you change it back.");
				return;
			}
			if (RepoRoles.showGUIAtStart.Value)
			{
				guiManager.showGUI = true;
			}
			else
			{
				guiManager.showGUI = false;
			}
			harmonyPatcher.PatchAll(typeof(PunManagerPatch));
			if (roleId <= 0)
			{
				mls.LogWarning((object)"Unable to find RoleId! Please contact the mod developer.");
				return;
			}
			KeyCode value7;
			switch (roleId)
			{
			case 1:
			{
				if (!RepoRoles.enableRunner.Value)
				{
					assignRole(rnd.Next(1, roleAmount + 1), PlayerController.instance);
					Debug.Log((object)"You got assigned a new random role because this one was disabled.");
					break;
				}
				mls.LogInfo((object)("Assigning role " + RepoRoles.customRoleNameRunner.Value + "."));
				__instance.CrouchSpeed = origCrouchSpeed * speedMultiplier;
				__instance.MoveSpeed = origMoveSpeed * speedMultiplier;
				__instance.SprintSpeed = origSprintSpeed * speedMultiplier + (float)num2 * 1f;
				__instance.EnergyStart = (origMaxEnergy + (float)num3 * 10f) * 1.2f;
				__instance.EnergyCurrent = (origMaxEnergy + (float)num3 * 10f) * 1.2f;
				RepoRoles.GUIinstance.text = RepoRoles.customRoleNameRunner.Value;
				RepoRoles.GUIinstance.color = new Color(0.973f, 1f, 0.196f);
				guiManager gUIinstance2 = RepoRoles.GUIinstance;
				string value8 = RepoRoles.customRoleDecRunner.Value;
				value7 = RepoRoles.toggleKey.Value;
				gUIinstance2.descText = value8 + "\nPress " + ((object)(KeyCode)(ref value7)).ToString() + " to continue";
				RepoRoles.GUIinstance.descColor = new Color(0.973f, 1f, 0.196f);
				break;
			}
			case 2:
			{
				if (!RepoRoles.enableTank.Value)
				{
					assignRole(rnd.Next(1, roleAmount + 1), PlayerController.instance);
					Debug.Log((object)"You got assigned a new random role because this one was disabled.");
					break;
				}
				mls.LogInfo((object)("Assigning role " + RepoRoles.customRoleNameTank.Value + "."));
				int num15 = (int)AccessTools.Field(typeof(PlayerHealth), "maxHealth").GetValue(PlayerAvatar.instance.playerHealth);
				int num16 = (int)AccessTools.Field(typeof(PlayerHealth), "health").GetValue(PlayerAvatar.instance.playerHealth);
				setHealth(PlayerController.instance.playerSteamID, num15 * 2, num16 * 2);
				__instance.CrouchSpeed = origCrouchSpeed * 0.9f;
				__instance.MoveSpeed = origMoveSpeed * 0.9f;
				__instance.SprintSpeed = (origSprintSpeed + (float)num2 * 1f) * 0.9f;
				isTank = true;
				RepoRoles.GUIinstance.text = RepoRoles.customRoleNameTank.Value;
				RepoRoles.GUIinstance.color = Color.gray;
				guiManager gUIinstance9 = RepoRoles.GUIinstance;
				string value12 = RepoRoles.customRoleDecTank.Value;
				value7 = RepoRoles.toggleKey.Value;
				gUIinstance9.descText = value12 + "\nPress " + ((object)(KeyCode)(ref value7)).ToString() + " to continue";
				RepoRoles.GUIinstance.descColor = Color.gray;
				break;
			}
			case 3:
			{
				if (!RepoRoles.enableGambler.Value)
				{
					assignRole(rnd.Next(1, roleAmount + 1), PlayerController.instance);
					Debug.Log((object)"You got assigned a new random role because this one was disabled.");
					break;
				}
				mls.LogInfo((object)("Assigning role " + RepoRoles.customRoleNameGambler.Value + "."));
				object[] array = genGamblerEffects();
				if ((int)array[1] == 0)
				{
					__instance.CrouchSpeed = origCrouchSpeed * 1.3f;
					__instance.MoveSpeed = origMoveSpeed * 1.3f;
					__instance.SprintSpeed = origSprintSpeed + (float)num2 * 1.3f;
				}
				else if ((int)array[1] == 1)
				{
					__instance.EnergyStart = (origMaxEnergy + (float)num3 * 10f) * 1.8f;
					__instance.EnergyCurrent = (origMaxEnergy + (float)num3 * 10f) * 1.8f;
				}
				else if ((int)array[1] == 2)
				{
					int num7 = (int)AccessTools.Field(typeof(PlayerHealth), "maxHealth").GetValue(PlayerAvatar.instance.playerHealth);
					int num8 = (int)AccessTools.Field(typeof(PlayerHealth), "health").GetValue(PlayerAvatar.instance.playerHealth);
					setHealth(PlayerController.instance.playerSteamID, (int)((double)num7 * 1.8), (int)((double)num8 * 1.8));
				}
				else if ((int)array[1] == 3)
				{
					modifyStrength(PlayerController.instance.playerSteamID, (origGrabStrength + (float)num4 * 0.2f) * 1.3f);
				}
				else if ((int)array[1] == 4)
				{
					__instance.JumpForce *= 1.5f;
				}
				if ((int)array[2] == 0)
				{
					__instance.CrouchSpeed = origCrouchSpeed;
					__instance.MoveSpeed = origMoveSpeed;
					__instance.SprintSpeed = origSprintSpeed + (float)num2 * 0.8f;
				}
				else if ((int)array[2] == 1)
				{
					__instance.EnergyStart = (origMaxEnergy + (float)num3 * 10f) * 0.8f;
					__instance.EnergyCurrent = (origMaxEnergy + (float)num3 * 10f) * 0.8f;
				}
				else if ((int)array[2] == 2)
				{
					int num9 = (int)AccessTools.Field(typeof(PlayerHealth), "maxHealth").GetValue(PlayerAvatar.instance.playerHealth);
					int num10 = (int)AccessTools.Field(typeof(PlayerHealth), "health").GetValue(PlayerAvatar.instance.playerHealth);
					setHealth(PlayerController.instance.playerSteamID, (int)((double)num9 * 0.8), (int)((double)num10 * 0.8));
				}
				else if ((int)array[2] == 3)
				{
					modifyStrength(PlayerController.instance.playerSteamID, (origGrabStrength + (float)num4 * 0.2f) * 0.8f);
				}
				else if ((int)array[2] == 4)
				{
					__instance.JumpForce *= 0.7f;
				}
				RepoRoles.GUIinstance.text = RepoRoles.customRoleNameGambler.Value;
				RepoRoles.GUIinstance.color = new Color(0.576f, 0f, 0.831f);
				RepoRoles.GUIinstance.descColor = new Color(0.576f, 0f, 0.831f);
				guiManager gUIinstance3 = RepoRoles.GUIinstance;
				string? obj = array[0]?.ToString();
				value7 = RepoRoles.toggleKey.Value;
				gUIinstance3.descText = obj + "\nPress " + ((object)(KeyCode)(ref value7)).ToString() + " to continue";
				break;
			}
			case 4:
			{
				if (!RepoRoles.enableStrongman.Value)
				{
					assignRole(rnd.Next(1, roleAmount + 1), PlayerController.instance);
					Debug.Log((object)"You got assigned a new random role because this one was disabled.");
					break;
				}
				mls.LogMessage((object)("Assigning role " + RepoRoles.customRoleNameStrongman.Value + "."));
				Debug.Log((object)("Strength before: " + PhysGrabber.instance.grabStrength));
				modifyStrength(PlayerController.instance.playerSteamID, (origGrabStrength + (float)num4 * 0.2f) * 1.5f + 0.5f);
				Debug.Log((object)("Strength after: " + PhysGrabber.instance.grabStrength));
				RepoRoles.GUIinstance.text = RepoRoles.customRoleNameStrongman.Value;
				RepoRoles.GUIinstance.color = new Color(0.761f, 0.055f, 0.055f);
				guiManager gUIinstance8 = RepoRoles.GUIinstance;
				string value11 = RepoRoles.customRoleDecStrongman.Value;
				value7 = RepoRoles.toggleKey.Value;
				gUIinstance8.descText = value11 + "\nPress " + ((object)(KeyCode)(ref value7)).ToString() + " to continue";
				RepoRoles.GUIinstance.descColor = new Color(0.761f, 0.055f, 0.055f);
				break;
			}
			case 5:
			{
				if (!RepoRoles.enableRL.Value)
				{
					assignRole(rnd.Next(1, roleAmount + 1), PlayerController.instance);
					Debug.Log((object)"You got assigned a new random role because this one was disabled.");
					break;
				}
				mls.LogMessage((object)("Assigning role " + RepoRoles.customRoleNameRL.Value + "."));
				PhysGrabber.instance.grabRange = (origGrabRange + (float)num5 * 1f) * 2.5f;
				modifyStrength(PlayerController.instance.playerSteamID, (origGrabStrength + (float)num4 * 0.2f) * 1.2f);
				RepoRoles.GUIinstance.text = RepoRoles.customRoleNameRL.Value;
				RepoRoles.GUIinstance.color = new Color(0.592f, 0.969f, 0.663f);
				guiManager gUIinstance5 = RepoRoles.GUIinstance;
				value7 = RepoRoles.toggleKey.Value;
				gUIinstance5.descText = "You can reach objects from far away!\nPress " + ((object)(KeyCode)(ref value7)).ToString() + " to continue";
				RepoRoles.GUIinstance.descColor = new Color(0.592f, 0.969f, 0.663f);
				break;
			}
			case 6:
			{
				if (!RepoRoles.enableAthletic.Value)
				{
					assignRole(rnd.Next(1, roleAmount + 1), PlayerController.instance);
					Debug.Log((object)"You got assigned a new random role because this one was disabled.");
					break;
				}
				mls.LogMessage((object)("Assigning role " + RepoRoles.customRoleNameAthletic.Value + "."));
				modifyStrength(PlayerController.instance.playerSteamID, (origGrabStrength + (float)num4 * 0.2f) * 1.3f);
				__instance.EnergyStart = origMaxEnergy + (float)num3 * 10f + 20f;
				__instance.EnergyCurrent = origMaxEnergy + (float)num3 * 10f + 20f;
				__instance.JumpForce = origJumpForce + 3f;
				RepoRoles.GUIinstance.text = RepoRoles.customRoleNameAthletic.Value;
				RepoRoles.GUIinstance.color = Color.white;
				guiManager gUIinstance6 = RepoRoles.GUIinstance;
				string value10 = RepoRoles.customRoleDecAthletic.Value;
				value7 = RepoRoles.toggleKey.Value;
				gUIinstance6.descText = value10 + "\nPress " + ((object)(KeyCode)(ref value7)).ToString() + " to continue";
				RepoRoles.GUIinstance.descColor = Color.white;
				break;
			}
			case 7:
			{
				if (!RepoRoles.enableMage.Value)
				{
					assignRole(rnd.Next(1, roleAmount + 1), PlayerController.instance);
					Debug.Log((object)"You got assigned a new random role because this one was disabled.");
					break;
				}
				mls.LogMessage((object)("Assigning role " + RepoRoles.customRoleNameMage.Value + "."));
				guiManager.isMage = true;
				guiManager.aviableMana = 8;
				ManaHelper.CreateUI();
				int num13 = (int)AccessTools.Field(typeof(PlayerHealth), "maxHealth").GetValue(PlayerAvatar.instance.playerHealth);
				int num14 = (int)AccessTools.Field(typeof(PlayerHealth), "health").GetValue(PlayerAvatar.instance.playerHealth);
				if ((double)num14 * 0.5 > 0.0)
				{
					setHealth(PlayerController.instance.playerSteamID, (int)((double)num13 * 0.5), (int)((double)num14 * 0.5));
				}
				else
				{
					setHealth(PlayerController.instance.playerSteamID, (int)((double)num13 * 0.5), num14);
				}
				RepoRoles.GUIinstance.text = RepoRoles.customRoleNameMage.Value;
				RepoRoles.GUIinstance.color = new Color(0f, 0.384f, 1f);
				guiManager gUIinstance7 = RepoRoles.GUIinstance;
				string[] obj2 = new string[6]
				{
					RepoRoles.customRoleDecMage.Value,
					"\nPress ",
					null,
					null,
					null,
					null
				};
				value7 = RepoRoles.showSpellsKey.Value;
				obj2[2] = ((object)(KeyCode)(ref value7)).ToString();
				obj2[3] = " to see all your spells and press ";
				value7 = RepoRoles.toggleKey.Value;
				obj2[4] = ((object)(KeyCode)(ref value7)).ToString();
				obj2[5] = " to continue";
				gUIinstance7.descText = string.Concat(obj2);
				RepoRoles.GUIinstance.descColor = new Color(0f, 0.384f, 1f);
				break;
			}
			case 8:
			{
				if (!RepoRoles.enableReaper.Value)
				{
					assignRole(rnd.Next(1, roleAmount + 1), PlayerController.instance);
					Debug.Log((object)"You got assigned a new random role because this one was disabled.");
					break;
				}
				mls.LogMessage((object)("Assigning role " + RepoRoles.customRoleNameReaper.Value + "."));
				harmonyPatcher.PatchAll(typeof(ReaperPatch));
				setReaperStatus(PlayerController.instance.playerSteamID, isReaper: true);
				int num11 = (int)AccessTools.Field(typeof(PlayerHealth), "maxHealth").GetValue(PlayerAvatar.instance.playerHealth);
				int num12 = (int)AccessTools.Field(typeof(PlayerHealth), "health").GetValue(PlayerAvatar.instance.playerHealth);
				setHealth(PlayerController.instance.playerSteamID, (int)((double)num11 * 1.5), (int)((double)num12 * 1.5));
				RepoRoles.GUIinstance.text = RepoRoles.customRoleNameReaper.Value;
				RepoRoles.GUIinstance.color = new Color(0.141f, 0.6f, 0.502f);
				guiManager gUIinstance4 = RepoRoles.GUIinstance;
				string value9 = RepoRoles.customRoleDecReaper.Value;
				value7 = RepoRoles.toggleKey.Value;
				gUIinstance4.descText = value9 + "\nPress " + ((object)(KeyCode)(ref value7)).ToString() + " to continue";
				RepoRoles.GUIinstance.descColor = new Color(0.141f, 0.6f, 0.502f);
				break;
			}
			case 9:
			{
				if (!RepoRoles.enableScout.Value)
				{
					assignRole(rnd.Next(1, roleAmount + 1), PlayerController.instance);
					Debug.Log((object)"You got assigned a new random role because this one was disabled.");
					break;
				}
				mls.LogMessage((object)("Assigning role " + RepoRoles.customRoleNameReaper.Value + "."));
				isScout = true;
				__instance.EnergyStart = (origMaxEnergy + (float)num3 * 10f) * 2f;
				__instance.EnergyCurrent = (origMaxEnergy + (float)num3 * 10f) * 2f;
				__instance.sprintRechargeAmount *= 2f;
				RepoRoles.GUIinstance.text = RepoRoles.customRoleNameScout.Value;
				RepoRoles.GUIinstance.color = new Color(0.902f, 0.733f, 0.11f);
				guiManager gUIinstance = RepoRoles.GUIinstance;
				string value6 = RepoRoles.customRoleDecScout.Value;
				value7 = RepoRoles.scoutKey.Value;
				string text2 = value6.Replace("[G]", "[" + ((object)(KeyCode)(ref value7)).ToString() + "]");
				value7 = RepoRoles.toggleKey.Value;
				gUIinstance.descText = text2 + "\nPress " + ((object)(KeyCode)(ref value7)).ToString() + " to continue";
				RepoRoles.GUIinstance.descColor = new Color(0.902f, 0.733f, 0.11f);
				break;
			}
			}
		}
	}
	[HarmonyPatch(typeof(EnemyHealth))]
	internal static class ReaperPatch
	{
		public static readonly Harmony harmonyPatcher = new Harmony("sender.reporoles.mod");

		private static ReaperManager rMan;

		[HarmonyPatch("Death")]
		[HarmonyPrefix]
		public static void PrefixMethod()
		{
			if ((Object)(object)((Component)SemiFunc.PlayerAvatarGetFromSteamID(PlayerController.instance.playerSteamID)).GetComponent<ReaperManager>() != (Object)null)
			{
				rMan = ((Component)SemiFunc.PlayerAvatarGetFromSteamID(PlayerController.instance.playerSteamID)).GetComponent<ReaperManager>();
			}
			else
			{
				Debug.LogError((object)"Failed to get Reaper Manager! Please contact the mod developer about this.");
			}
			if (rMan.isReaper && (Object)(object)rMan != (Object)null)
			{
				rMan.kills++;
			}
			else
			{
				Debug.LogError((object)"Unable to find ReaperManager in PlayerAvatar.instance. Please report this to the mod author.");
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
			photonView = ((Component)this).GetComponent<PhotonView>();
		}

		[PunRPC]
		internal void setReaperStatusRPC(string steamID, bool isReaper)
		{
			PlayerAvatar val = SemiFunc.PlayerAvatarGetFromSteamID(steamID);
			if (Object.op_Implicit((Object)(object)val))
			{
				((Component)val).GetComponent<ReaperManager>().isReaper = isReaper;
			}
		}

		private void Update()
		{
			foreach (PlayerAvatar item in SemiFunc.PlayerGetAll())
			{
				if (!((Object)(object)((Component)item).GetComponent<ReaperManager>() != (Object)null) || !((Component)item).GetComponent<ReaperManager>().isReaper || (int)AccessTools.Field(typeof(PlayerHealth), "health").GetValue(item.playerHealth) <= 0)
				{
					continue;
				}
				if (!SemiFunc.IsMultiplayer())
				{
					if (((Component)SemiFunc.PlayerAvatarGetFromSteamID(PlayerController.instance.playerSteamID)).GetComponent<ReaperManager>().kills > 0 && enemyDeathTimer <= 0)
					{
						AccessTools.Field(typeof(PlayerHealth), "maxHealth").SetValue(item.playerHealth, (int)AccessTools.Field(typeof(PlayerHealth), "maxHealth").GetValue(item.playerHealth) + 5);
						if ((int)AccessTools.Field(typeof(PlayerHealth), "health").GetValue(item.playerHealth) + 30 > (int)AccessTools.Field(typeof(PlayerHealth), "maxHealth").GetValue(item.playerHealth))
						{
							item.playerHealth.Heal((int)AccessTools.Field(typeof(PlayerHealth), "maxHealth").GetValue(item.playerHealth) - (int)AccessTools.Field(typeof(PlayerHealth), "health").GetValue(item.playerHealth), true);
						}
						else
						{
							item.playerHealth.Heal(30, true);
						}
						((Component)item).GetComponent<ReaperManager>().kills = 0;
						enemyDeathTimer = 50;
					}
				}
				else if ((Object)(object)item.photonView != (Object)null)
				{
					item.photonView.RPC("giveReaperStatsRPC", (RpcTarget)0, new object[1] { item.steamID });
				}
			}
			if (enemyDeathTimer > 0)
			{
				enemyDeathTimer--;
			}
		}

		[PunRPC]
		internal void giveReaperStatsRPC(string steamID)
		{
			PlayerAvatar val = SemiFunc.PlayerAvatarGetFromSteamID(steamID);
			if (Object.op_Implicit((Object)(object)val) && ((Component)SemiFunc.PlayerAvatarGetFromSteamID(PlayerController.instance.playerSteamID)).GetComponent<ReaperManager>().kills > 0 && enemyDeathTimer <= 0)
			{
				AccessTools.Field(typeof(PlayerHealth), "maxHealth").SetValue(val.playerHealth, (int)AccessTools.Field(typeof(PlayerHealth), "maxHealth").GetValue(val.playerHealth) + 5);
				if ((int)AccessTools.Field(typeof(PlayerHealth), "health").GetValue(val.playerHealth) + 30 > (int)AccessTools.Field(typeof(PlayerHealth), "maxHealth").GetValue(val.playerHealth))
				{
					val.playerHealth.Heal((int)AccessTools.Field(typeof(PlayerHealth), "maxHealth").GetValue(val.playerHealth) - (int)AccessTools.Field(typeof(PlayerHealth), "health").GetValue(val.playerHealth), true);
				}
				else
				{
					val.playerHealth.Heal(30, true);
				}
				((Component)val).GetComponent<ReaperManager>().kills = 0;
				enemyDeathTimer = 50;
			}
		}
	}
	[HarmonyPatch(typeof(TruckScreenText))]
	internal static class Sender
	{
		private static bool hasRanOnce;

		public static ClassManager manager = new ClassManager();

		public static readonly Harmony harmonyPatcher = new Harmony("sender.reporoles.mod");

		[HarmonyPatch("ArrowPointAtGoalLogic")]
		[HarmonyPrefix]
		private static void PrefixMethod()
		{
			if (!hasRanOnce)
			{
				manager.assignRoleFromConfig(PlayerController.instance);
				Logger.CreateLogSource("ChatMsgMgr").LogInfo((object)"Successfully rolled role!");
				hasRanOnce = true;
			}
		}

		[HarmonyPatch("Start")]
		[HarmonyPrefix]
		private static void StartPrefix()
		{
			hasRanOnce = false;
		}
	}
	[HarmonyPatch(typeof(PlayerAvatar))]
	internal static class PlayerAvatarPatch
	{
		[HarmonyPostfix]
		[HarmonyPatch("Start")]
		private static void Start_Postfix(PlayerAvatar __instance)
		{
			if ((Object)(object)((Component)__instance).GetComponent<ReaperManager>() == (Object)null)
			{
				ComponentHolderProtocol.AddComponent<ReaperManager>((Object)(object)__instance);
			}
		}
	}
}
namespace R.E.P.O.Roles.patches
{
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

		private float multiSpeed = 1.4f;

		private float crouchSpeedBefore;

		private float moveSpeedBefore;

		private float sprintSpeedBefore;

		private int overchargeMulti = 1;

		private bool isOvercharged = false;

		private int overchargeTicker = 0;

		private bool jumpActive = false;

		private float jumpOriginal;

		private int jumpTicker = 0;

		private int neededJumpTicker = 1800;

		private int neededSpeedTicker = 1800;

		private int manaUsageTicker = 0;

		private bool showManaUsage = false;

		private Texture2D textureRunner;

		private string manaUsageText = "Used x (- y Mana)";

		private bool fontLoaded = false;

		private bool RunnerTextureLoaded = false;

		private int passiveTankRegenTicker = 0;

		public static AssetBundle LoadAssetBundle(string name)
		{
			Logger.CreateLogSource("AssetBundles").LogDebug((object)("Loading Asset Bundle: " + name));
			AssetBundle val = null;
			string text = Path.Combine(Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location), name);
			return AssetBundle.LoadFromFile(text);
		}

		public static Texture2D LoadBottleTextureFromFile()
		{
			AssetBundle val = LoadAssetBundle("REPORoles_assets1");
			Texture2D result = val.LoadAsset<Texture2D>("emptyBottle.png");
			val.Unload(false);
			return result;
		}

		public static Texture2D LoadFullBottleTextureFromFile()
		{
			AssetBundle val = LoadAssetBundle("REPORoles_assets1");
			Texture2D result = val.LoadAsset<Texture2D>("filledBottle.png");
			val.Unload(false);
			return result;
		}

		public static Texture2D LoadTextureByName(string bundleName, string assetName)
		{
			AssetBundle val = LoadAssetBundle(bundleName);
			Texture2D result = val.LoadAsset<Texture2D>(assetName);
			val.Unload(false);
			return result;
		}

		public static void loadFont()
		{
			if ((Object)(object)fontBundle == (Object)null)
			{
				fontBundle = LoadAssetBundle("REPORoles_assets1");
				customFont = fontBundle.LoadAsset<Font>("font.ttf");
			}
			if ((Object)(object)fontBundle == (Object)null)
			{
				Logger.CreateLogSource("RepoRoles ERROR").LogError((object)"Missing Assets: Put the assets file next to the dll in your plugins folder. If this does not fix it contact us on our discord (see thunderstore description or README)");
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
			float num = Screen.width / 2 - 100;
			float num2 = Screen.height / 2 - 60;
			if (!fontLoaded)
			{
				loadFont();
				fontLoaded = true;
			}
			if (SemiFunc.RunIsLevel() && !SemiFunc.RunIsShop() && showGUI)
			{
				GUIStyle val = new GUIStyle();
				val.fontSize = 60;
				val.fontStyle = (FontStyle)1;
				val.richText = true;
				val.normal.textColor = color;
				val.alignment = (TextAnchor)4;
				val.font = customFont;
				GUIStyle val2 = new GUIStyle(val);
				val2.normal.textColor = Color.black;
				val2.alignment = (TextAnchor)4;
				val2.font = customFont;
				float num3 = 2f;
				GUI.Label(new Rect(num - num3, num2, 200f, 50f), this.text, val2);
				GUI.Label(new Rect(num + num3, num2, 200f, 50f), this.text, val2);
				GUI.Label(new Rect(num, num2 - num3, 200f, 50f), this.text, val2);
				GUI.Label(new Rect(num, num2 + num3, 200f, 50f), this.text, val2);
				GUI.Label(new Rect(num - num3, num2 - num3, 200f, 50f), this.text, val2);
				GUI.Label(new Rect(num - num3, num2 + num3, 200f, 50f), this.text, val2);
				GUI.Label(new Rect(num + num3, num2 - num3, 200f, 50f), this.text, val2);
				GUI.Label(new Rect(num + num3, num2 + num3, 200f, 50f), this.text, val2);
				GUI.Label(new Rect(num, num2, 200f, 50f), this.text, val);
				float num4 = Screen.width / 2 - 100;
				float num5 = Screen.height / 2 + 40;
				GUIStyle val3 = new GUIStyle();
				val3.fontSize = 40;
				val3.fontStyle = (FontStyle)1;
				val3.richText = true;
				val3.normal.textColor = color;
				val3.alignment = (TextAnchor)4;
				val3.font = customFont;
				GUIStyle val4 = new GUIStyle(val3);
				val4.normal.textColor = Color.black;
				val4.alignment = (TextAnchor)4;
				val4.font = customFont;
				float num6 = 2f;
				GUI.Label(new Rect(num4 - num6, num5, 200f, 50f), descText, val4);
				GUI.Label(new Rect(num4 + num6, num5, 200f, 50f), descText, val4);
				GUI.Label(new Rect(num4, num5 - num6, 200f, 50f), descText, val4);
				GUI.Label(new Rect(num4, num5 + num6, 200f, 50f), descText, val4);
				GUI.Label(new Rect(num4 - num6, num5 - num6, 200f, 50f), descText, val4);
				GUI.Label(new Rect(num4 - num6, num5 + num6, 200f, 50f), descText, val4);
				GUI.Label(new Rect(num4 + num6, num5 - num6, 200f, 50f), descText, val4);
				GUI.Label(new Rect(num4 + num6, num5 + num6, 200f, 50f), descText, val4);
				GUI.Label(new Rect(num4, num5, 200f, 50f), descText, val3);
			}
			if (!isMage || !SemiFunc.RunIsLevel() || SemiFunc.RunIsShop() || (int)AccessTools.Field(typeof(PlayerHealth), "health").GetValue(PlayerAvatar.instance.playerHealth) <= 0)
			{
				return;
			}
			GUIStyle val5 = new GUIStyle();
			val5.alignment = (TextAnchor)1;
			val5.normal.background = manaTexture;
			val5.font = customFont;
			GUIStyle val6 = new GUIStyle();
			val6.alignment = (TextAnchor)1;
			val6.normal.background = _backgroundTexture;
			val6.font = customFont;
			if (showManaUsage && SemiFunc.RunIsLevel() && !SemiFunc.RunIsShop() && isMage)
			{
				int num7 = Screen.width / 2 - 100;
				int num8 = Screen.height - 125;
				GUIStyle val7 = new GUIStyle();
				val7.fontSize = 20;
				val7.fontStyle = (FontStyle)1;
				val7.richText = true;
				val7.normal.textColor = new Color(0f, 0.384f, 1f);
				val7.alignment = (TextAnchor)4;
				val7.font = customFont;
				GUIStyle val8 = new GUIStyle(val7);
				val8.normal.textColor = Color.black;
				val8.alignment = (TextAnchor)4;
				val8.font = customFont;
				float num9 = 1.5f;
				GUI.Label(new Rect((float)num7 - num9, (float)num8, 200f, 50f), manaUsageText, val8);
				GUI.Label(new Rect((float)num7 + num9, (float)num8, 200f, 50f), manaUsageText, val8);
				GUI.Label(new Rect((float)num7, (float)num8 - num9, 200f, 50f), manaUsageText, val8);
				GUI.Label(new Rect((float)num7, (float)num8 + num9, 200f, 50f), manaUsageText, val8);
				GUI.Label(new Rect((float)num7 - num9, (float)num8 - num9, 200f, 50f), manaUsageText, val8);
				GUI.Label(new Rect((float)num7 - num9, (float)num8 + num9, 200f, 50f), manaUsageText, val8);
				GUI.Label(new Rect((float)num7 + num9, (float)num8 - num9, 200f, 50f), manaUsageText, val8);
				GUI.Label(new Rect((float)num7 + num9, (float)num8 + num9, 200f, 50f), manaUsageText, val8);
				GUI.Label(new Rect((float)num7, (float)num8, 200f, 50f), manaUsageText, val7);
			}
			if (SemiFunc.RunIsLevel() && !SemiFunc.RunIsShop() && isMage && RepoRoles.mageTopManaBool)
			{
				if ((Object)(object)ManaHelper.val == (Object)null)
				{
					ManaHelper.val = GameObject.Find("Energy");
				}
				GUI.Box(new Rect(num - 100f, 50f, 50f, 50f), GUIContent.none, val6);
				GUI.Box(new Rect(num - 50f, 50f, 50f, 50f), GUIContent.none, val6);
				GUI.Box(new Rect(num, 50f, 50f, 50f), GUIContent.none, val6);
				GUI.Box(new Rect(num + 50f, 50f, 50f, 50f), GUIContent.none, val6);
				GUI.Box(new Rect(num + 100f, 50f, 50f, 50f), GUIContent.none, val6);
				GUI.Box(new Rect(num + 150f, 50f, 50f, 50f), GUIContent.none, val6);
				GUI.Box(new Rect(num + 200f, 50f, 50f, 50f), GUIContent.none, val6);
				GUI.Box(new Rect(num + 250f, 50f, 50f, 50f), GUIContent.none, val6);
				if (aviableMana >= 1)
				{
					GUI.Box(new Rect(num - 100f, 50f, 50f, 50f), GUIContent.none, val5);
				}
				if (aviableMana >= 2)
				{
					GUI.Box(new Rect(num - 50f, 50f, 50f, 50f), GUIContent.none, val5);
				}
				if (aviableMana >= 3)
				{
					GUI.Box(new Rect(num, 50f, 50f, 50f), GUIContent.none, val5);
				}
				if (aviableMana >= 4)
				{
					GUI.Box(new Rect(num + 50f, 50f, 50f, 50f), GUIContent.none, val5);
				}
				if (aviableMana >= 5)
				{
					GUI.Box(new Rect(num + 100f, 50f, 50f, 50f), GUIContent.none, val5);
				}
				if (aviableMana >= 6)
				{
					GUI.Box(new Rect(num + 150f, 50f, 50f, 50f), GUIContent.none, val5);
				}
				if (aviableMana >= 7)
				{
					GUI.Box(new Rect(num + 200f, 50f, 50f, 50f), GUIContent.none, val5);
				}
				if (aviableMana >= 8)
				{
					GUI.Box(new Rect(num + 250f, 50f, 50f, 50f), GUIContent.none, val5);
				}
			}
			if (showSpells && SemiFunc.RunIsLevel() && !SemiFunc.RunIsShop())
			{
				GUIStyle val9 = new GUIStyle();
				val9.fontSize = 40;
				val9.fontStyle = (FontStyle)1;
				val9.richText = true;
				val9.normal.textColor = new Color(0f, 0.384f, 1f);
				val9.alignment = (TextAnchor)4;
				val9.font = customFont;
				GUIStyle val10 = new GUIStyle(val9);
				val10.normal.textColor = Color.black;
				val10.alignment = (TextAnchor)4;
				val10.font = customFont;
				string[] obj = new string[11]
				{
					"<size=65>SPELLS</size>\n[", null, null, null, null, null, null, null, null, null,
					null
				};
				KeyCode value = RepoRoles.healKey.Value;
				obj[1] = ((object)(KeyCode)(ref value)).ToString();
				obj[2] = "] Heal yourself for 5 health (1 Mana)\n\n[";
				value = RepoRoles.staminaKey.Value;
				obj[3] = ((object)(KeyCode)(ref value)).ToString();
				obj[4] = "] Regenerate your stamina to full (3 Mana)\n\n[";
				value = RepoRoles.speedKey.Value;
				obj[5] = ((object)(KeyCode)(ref value)).ToString();
				obj[6] = "] Become faster for 30 seconds (2 Mana)\n\n[";
				value = RepoRoles.overchargeKey.Value;
				obj[7] = ((object)(KeyCode)(ref value)).ToString();
				obj[8] = "] Boost the effectivity of spells used in the next 20 seconds (3 Mana)\n\n[";
				value = RepoRoles.jumpKey.Value;
				obj[9] = ((object)(KeyCode)(ref value)).ToString();
				obj[10] = "] Jump higher for 30 seconds (2 Mana)";
				string text = string.Concat(obj);
				float num10 = 2f;
				GUI.Label(new Rect(num - num10, num2, 200f, 50f), text, val10);
				GUI.Label(new Rect(num + num10, num2, 200f, 50f), text, val10);
				GUI.Label(new Rect(num, num2 - num10, 200f, 50f), text, val10);
				GUI.Label(new Rect(num, num2 + num10, 200f, 50f), text, val10);
				GUI.Label(new Rect(num - num10, num2 - num10, 200f, 50f), text, val10);
				GUI.Label(new Rect(num - num10, num2 + num10, 200f, 50f), text, val10);
				GUI.Label(new Rect(num + num10, num2 - num10, 200f, 50f), text, val10);
				GUI.Label(new Rect(num + num10, num2 + num10, 200f, 50f), text, val10);
				GUI.Label(new Rect(num, num2, 200f, 50f), text, val9);
			}
		}

		private void updateMana()
		{
			if (isMage)
			{
				ManaHelper.val2.gameObject.GetComponent<ManaUI>().SetMana(aviableMana, 8f);
			}
		}

		private void Update()
		{
			if (SemiFunc.RunIsLevel() && (int)AccessTools.Field(typeof(PlayerHealth), "health").GetValue(PlayerAvatar.instance.playerHealth) <= 0)
			{
				isDead = true;
			}
			if (RepoRoles.mageTopManaBool && isMage && (Object)(object)ManaHelper.val != (Object)null && (Object)(object)ManaHelper.val2 != (Object)null && ManaHelper.val2.activeInHierarchy)
			{
				ManaHelper.val2.SetActive(false);
			}
			else if (!RepoRoles.mageTopManaBool && isMage && (Object)(object)ManaHelper.val != (Object)null && (Object)(object)ManaHelper.val2 != (Object)null && !ManaHelper.val2.activeInHierarchy)
			{
				ManaHelper.val2.SetActive(true);
				((Behaviour)ManaHelper.val2.gameObject.GetComponent<ManaUI>().Text).enabled = true;
			}
			if (!SemiFunc.RunIsLevel())
			{
				isMage = false;
				if (Sender.manager != null && (Object)(object)PlayerAvatar.instance != (Object)null)
				{
					Sender.manager.setReaperStatus(PlayerController.instance.playerSteamID, isReaper: false);
				}
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
				Logger.CreateLogSource("REPO Roles guiManager").LogInfo((object)"Assigning new role on respawn!");
				isDead = false;
			}
			if (Input.GetKeyDown(RepoRoles.toggleKey.Value) && !ChatManager.instance.chatActive)
			{
				showGUI = !showGUI;
			}
			if ((int)AccessTools.Field(typeof(PlayerHealth), "health").GetValue(PlayerAvatar.instance.playerHealth) > 0 && isMage && !ChatManager.instance.chatActive)
			{
				if (Input.GetKeyDown(RepoRoles.showSpellsKey.Value))
				{
					showSpells = !showSpells;
				}
				if (Input.GetKeyDown(RepoRoles.healKey.Value))
				{
					if ((int)AccessTools.Field(typeof(PlayerHealth), "health").GetValue(PlayerAvatar.instance.playerHealth) + 5 * overchargeMulti < (int)AccessTools.Field(typeof(PlayerHealth), "maxHealth").GetValue(PlayerAvatar.instance.playerHealth))
					{
						if (aviableMana >= 1)
						{
							PlayerAvatar.instance.playerHealth.Heal(5 * overchargeMulti, true);
							aviableMana--;
							showManaUsage = true;
							manaUsageTicker = 0;
							manaUsageText = "Used Heal (-1 Mana)";
							updateMana();
						}
					}
					else if ((int)AccessTools.Field(typeof(PlayerHealth), "health").GetValue(PlayerAvatar.instance.playerHealth) != (int)AccessTools.Field(typeof(PlayerHealth), "maxHealth").GetValue(PlayerAvatar.instance.playerHealth) && aviableMana >= 1)
					{
						PlayerAvatar.instance.playerHealth.Heal((int)AccessTools.Field(typeof(PlayerHealth), "maxHealth").GetValue(PlayerAvatar.instance.playerHealth) - (int)AccessTools.Field(typeof(PlayerHealth), "health").GetValue(PlayerAvatar.instance.playerHealth), true);
						aviableMana--;
						showManaUsage = true;
						manaUsageTicker = 0;
						manaUsageText = "Used Heal (-1 Mana)";
						updateMana();
					}
				}
				if (Input.GetKeyDown(RepoRoles.speedKey.Value) && !speedActive)
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
						aviableMana -= 2;
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
				if (Input.GetKeyDown(RepoRoles.overchargeKey.Value) && !isOvercharged && aviableMana >= 3)
				{
					aviableMana -= 3;
					isOvercharged = true;
					overchargeMulti = 2;
					overchargeTicker = 0;
					showManaUsage = true;
					manaUsageTicker = 0;
					manaUsageText = "Used Overcharge (-3 Mana)";
					updateMana();
				}
				if (Input.GetKeyDown(RepoRoles.jumpKey.Value) && !jumpActive)
				{
					jumpOriginal = PlayerController.instance.JumpForce;
					if (aviableMana >= 2)
					{
						jumpTicker = 0;
						jumpActive = true;
						PlayerController.instance.JumpForce = jumpOriginal + 3f;
						aviableMana -= 2;
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
				if (Input.GetKeyDown(RepoRoles.staminaKey.Value) && PlayerController.instance.EnergyCurrent < PlayerController.instance.EnergyStart && aviableMana >= 3)
				{
					PlayerController.instance.EnergyCurrent = PlayerController.instance.EnergyStart;
					aviableMana -= 3;
					showManaUsage = true;
					manaUsageTicker = 0;
					manaUsageText = "Used Stamina Refill (-3 Mana)";
					updateMana();
				}
			}
			if (speedActive && speedTicker >= neededSpeedTicker)
			{
				PlayerController.instance.CrouchSpeed = crouchSpeedBefore;
				PlayerController.instance.MoveSpeed = moveSpeedBefore;
				PlayerController.instance.SprintSpeed = sprintSpeedBefore;
				speedActive = false;
			}
			if (jumpActive && jumpTicker >= neededJumpTicker)
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
				return;
			}
			PunManager.instance.photonView.RPC("setHealthRPC", (RpcTarget)0, new object[3] { steamID, maxHealth, health });
		}

		private void FixedUpdate()
		{
			if (passiveTankRegenTicker < 250 && ClassManager.isTank && SemiFunc.RunIsLevel() && !SemiFunc.RunIsShop() && !isDead)
			{
				passiveTankRegenTicker++;
			}
			if (passiveTankRegenTicker >= 250 && SemiFunc.RunIsLevel() && !SemiFunc.RunIsShop() && ClassManager.isTank)
			{
				passiveTankRegenTicker = 0;
				int num = (int)AccessTools.Field(typeof(PlayerHealth), "maxHealth").GetValue(PlayerAvatar.instance.playerHealth);
				int num2 = (int)AccessTools.Field(typeof(PlayerHealth), "health").GetValue(PlayerAvatar.instance.playerHealth);
				if (num2 < num)
				{
					setHealth(PlayerController.instance.playerSteamID, num, num2 + 1);
				}
			}
			if (overchargeTicker >= 1200)
			{
				isOvercharged = false;
				overchargeMulti = 1;
			}
			if (isOvercharged)
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
			photonView = ((Component)this).GetComponent<PhotonView>();
		}

		[PunRPC]
		internal void setStrengthRPC(string steamID, float newStrength)
		{
			PlayerAvatar val = SemiFunc.PlayerAvatarGetFromSteamID(steamID);
			if (Object.op_Implicit((Object)(object)val))
			{
				val.physGrabber.grabStrength = newStrength;
			}
		}
	}
	public class HealthManager : MonoBehaviour
	{
		internal PhotonView photonView;

		private void Start()
		{
			photonView = ((Component)this).GetComponent<PhotonView>();
		}

		[PunRPC]
		internal void setHealthRPC(string steamID, int maxHealth, int health)
		{
			PlayerAvatar val = SemiFunc.PlayerAvatarGetFromSteamID(steamID);
			if (Object.op_Implicit((Object)(object)val))
			{
				val.playerHealth.maxHealth = maxHealth;
				val.playerHealth.health = health;
			}
		}
	}
	public class Upgrader : MonoBehaviour
	{
		private ItemToggle _itemToggle;

		private void Start()
		{
			_itemToggle = ((Component)this).GetComponent<ItemToggle>();
		}

		public static void UpdateStat(int amount, string steamId, string stat)
		{
			Dictionary<string, int> dictionary = StatsManager.instance.dictionaryOfDictionaries[stat];
			if (!dictionary.ContainsKey(steamId))
			{
				dictionary[steamId] = 0;
			}
			dictionary[steamId] += amount;
		}

		public static int GetStat(string steamId, string upgradeName)
		{
			Dictionary<string, int> dictionary = StatsManager.instance.dictionaryOfDictionaries[upgradeName];
			int value;
			return dictionary.TryGetValue(steamId, out value) ? value : 0;
		}

		public void upgrade()
		{
			string steamId = SemiFunc.PlayerGetSteamID(SemiFunc.PlayerAvatarGetFromPhotonID(_itemToggle.playerTogglePhotonID));
			Debug.Log((object)("Your Mana Regen before: " + GetStat(steamId, "playerUpgradeManaRegeneration")));
			UpdateStat(1, steamId, "playerUpgradeManaRegeneration");
			RepoRoles.Update_ManaRegeneration();
			Debug.Log((object)("Your Mana Regen after: " + GetStat(steamId, "playerUpgradeManaRegeneration")));
		}

		public void upgradeScoutCooldown()
		{
			string steamId = SemiFunc.PlayerGetSteamID(SemiFunc.PlayerAvatarGetFromPhotonID(_itemToggle.playerTogglePhotonID));
			Debug.Log((object)("Your Scout Cooldown Upgrades before: " + GetStat(steamId, "playerUpgradeScoutCooldownReduction")));
			UpdateStat(1, steamId, "playerUpgradeScoutCooldownReduction");
			RepoRoles.Update_ScoutCooldown();
			Debug.Log((object)("Your Scout Cooldown Upgrades after: " + GetStat(steamId, "playerUpgradeScoutCooldownReduction")));
		}
	}
	[HarmonyPatch(typeof(StatsManager), "Start")]
	internal static class StatsManagerPatch
	{
		private static void Prefix(StatsManager __instance)
		{
			__instance.dictionaryOfDictionaries.Add("playerUpgradeManaRegeneration", new Dictionary<string, int>());
			__instance.dictionaryOfDictionaries.Add("playerUpgradeScoutCooldownReduction", new Dictionary<string, int>());
		}
	}
	public class ManaUI : SemiUI
	{
		public TextMeshProUGUI Text;

		public static ManaUI instance;

		public TextMeshProUGUI textManaMax;

		public override void Start()
		{
			((SemiUI)this).Start();
			instance = this;
			Text = ((Component)this).GetComponent<TextMeshProUGUI>();
			((TMP_Text)Text).text = "0" + Mathf.Ceil((float)guiManager.aviableMana);
		}

		public void SetMana(float mana, float maxMana)
		{
			Text = ((Component)this).GetComponent<TextMeshProUGUI>();
			textManaMax = ((Component)((Component)this).transform.Find("EnergyMax")).GetComponent<TextMeshProUGUI>();
			if ((Object)(object)ManaHelper.val == (Object)null)
			{
				ManaHelper.val = GameObject.Find("Energy");
			}
			if (!((Object)(object)Text == (Object)null) && !((Object)(object)textManaMax == (Object)null))
			{
				((TMP_Text)Text).text = "0" + Mathf.Ceil(mana);
				((TMP_Text)textManaMax).text = "<b>/</b>0" + Mathf.Ceil(maxMana);
			}
		}
	}
	public class ManaHelper : MonoBehaviour
	{
		public static GameObject val;

		public static GameObject val2;

		public static ManaUI manaUI;

		public static void CreateUI()
		{
			ManaHelper.val = GameObject.Find("Energy");
			val2 = Object.Instantiate<GameObject>(ManaHelper.val, ManaHelper.val.transform.parent);
			((Object)val2).name = "Mana";
			Transform transform = val2.transform;
			transform.localPosition -= new Vector3(0f, 33f, 0f);
			EnergyUI component = val2.GetComponent<EnergyUI>();
			if ((Object)(object)component != (Object)null)
			{
				Object.DestroyImmediate((Object)(object)component);
			}
			TextMeshProUGUI[] componentsInChildren = val2.GetComponentsInChildren<TextMeshProUGUI>();
			TextMeshProUGUI[] array = componentsInChildren;
			TextMeshProUGUI[] array2 = array;
			foreach (TextMeshProUGUI val in array2)
			{
				((Graphic)val).color = new Color(0f, 0.384f, 1f);
			}
			Texture2D manaTexture = guiManager.manaTexture;
			Sprite sprite = Sprite.Create(manaTexture, new Rect(0f, 0f, (float)((Texture)manaTexture).width, (float)((Texture)manaTexture).height), new Vector2(0.5f, 0.5f));
			((Component)val2.transform).gameObject.GetComponentInChildren<Image>().sprite = sprite;
			((Graphic)((Component)val2.transform).gameObject.GetComponentInChildren<Image>()).color = Color.white;
			manaUI = val2.AddComponent<ManaUI>();
			manaUI.SetMana(8f, 8f);
		}
	}
}
